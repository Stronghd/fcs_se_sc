#include "Student.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <utility>

#include <unicode/uchar.h>
#include <unicode/utf8.h>

bool Student::nameCheck(const std::string& name)
{
    if (name.empty() ||
        name.size() > static_cast<std::size_t>(std::numeric_limits<std::int32_t>::max()))
    {
        return false;
    }

    const auto* bytes = reinterpret_cast<const std::uint8_t*>(name.data());
    const auto length = static_cast<std::int32_t>(name.size());
    std::int32_t offset = 0;
    while (offset < length)
    {
        UChar32 codePoint;
        U8_NEXT(bytes, offset, length, codePoint);
        if (codePoint < 0 || !u_isalpha(codePoint))
        {
            return false;
        }
    }
    return true;
}

Student::Student(std::uint64_t id, std::string name, std::string surname)
    : id_(id), name_(std::move(name)), surname_(std::move(surname))
{
    if (!nameCheck(name_))
    {
        throw std::invalid_argument("invalid name");
    }
    if (!nameCheck(surname_))
    {
        throw std::invalid_argument("invalid surname");
    }
}

std::uint64_t Student::getId() const
{
    return id_;
}

const std::string& Student::getName() const
{
    return name_;
}

const std::string& Student::getSurname() const
{
    return surname_;
}
