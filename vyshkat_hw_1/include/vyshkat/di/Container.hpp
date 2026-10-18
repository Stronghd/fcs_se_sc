#pragma once

#include <functional>
#include <memory>
#include <stdexcept>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>


namespace vyshkat {
class Container {
public:
    template <class I>
    void registerSingleton(std::function<std::shared_ptr<I>(Container &)> factory) {
        factories_[std::type_index(typeid(I))] = [factory](Container &c) -> std::shared_ptr<void> {
            return factory(c);
        };
    }

    template <class I>
    std::shared_ptr<I> resolve() {
        const std::type_index key(typeid(I));

        auto cached = instances_.find(key);
        if (cached != instances_.end()) {
            return std::static_pointer_cast<I>(cached->second);
        }

        auto factory = factories_.find(key);
        if (factory == factories_.end()) {
            throw std::runtime_error("не зарегистрирован: " + std::string(typeid(I).name()));
        }

        std::shared_ptr<void> instance = factory->second(*this);
        instances_[key] = instance;
        return std::static_pointer_cast<I>(instance);
    }

private:
    std::unordered_map<std::type_index, std::function<std::shared_ptr<void>(Container &)>> factories_;
    std::unordered_map<std::type_index, std::shared_ptr<void>> instances_;
};


} // namespace vyshkat