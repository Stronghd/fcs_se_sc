#pragma once

#include <functional>
#include <map>
#include <memory>
#include <stdexcept>
#include <typeindex>
#include <utility>

class DependencyContainer
{
public:
    template <typename Service, typename Factory>
    void bind(Factory factory)
    {
        const auto key = std::type_index(typeid(Service));
        const bool inserted = registrations_.emplace(key, Registration{
            [factory = std::move(factory)](DependencyContainer& container) {
                std::shared_ptr<Service> service = factory(container);
                return std::shared_ptr<void>(std::move(service));
            }, {}, false
        }).second;
        if (!inserted)
        {
            throw std::logic_error("Service is already registered");
        }
    }

    template <typename Service>
    Service& resolve()
    {
        const auto found = registrations_.find(std::type_index(typeid(Service)));
        if (found == registrations_.end())
        {
            throw std::logic_error("Service is not registered");
        }

        auto& registration = found->second;
        if (registration.constructing)
        {
            throw std::logic_error("Circular service dependency");
        }

        if (!registration.instance)
        {
            registration.constructing = true;
            try
            {
                registration.instance = registration.factory(*this);
                registration.constructing = false;
            }
            catch (...)
            {
                registration.constructing = false;
                throw;
            }
            if (!registration.instance)
            {
                throw std::logic_error("Service factory returned an empty instance");
            }
        }

        return *static_cast<Service*>(registration.instance.get());
    }

private:
    struct Registration
    {
        std::function<std::shared_ptr<void>(DependencyContainer&)> factory;
        std::shared_ptr<void> instance;
        bool constructing;
    };

    std::map<std::type_index, Registration> registrations_;
};
