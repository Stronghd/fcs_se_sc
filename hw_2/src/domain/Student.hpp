#pragma once

#include <string>
#include <cstdint>

class Student
{
public:
    Student(std::uint64_t id, std::string name, std::string surname);

    std::uint64_t getId() const;
    const std::string& getName() const;
    const std::string& getSurname() const;

private:
    std::uint64_t id_;
    std::string name_;
    std::string surname_;

    static bool nameCheck(const std::string& name);
};
