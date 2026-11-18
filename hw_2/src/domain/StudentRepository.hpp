#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Student.hpp"

class StudentRepository
{
public:
    virtual ~StudentRepository() = default;

    virtual std::uint64_t addStudent(std::string name, std::string surname) = 0;
    virtual const Student* findStudent(const std::uint64_t& id) const = 0;
    virtual std::vector<Student> getStudents() const = 0;
};
