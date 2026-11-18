#pragma once

#include <map>
#include <cstdint>
#include <string>
#include <vector>

#include "StudentRepository.hpp"

class StudentRegistry final : public StudentRepository
{
public:
    std::uint64_t addStudent(std::string name, std::string surname) override;
    const Student *findStudent(const std::uint64_t &id) const override;
    std::vector<Student> getStudents() const override;

private:
    std::map<std::uint64_t, Student> students_;
    std::uint64_t nextId_ = 1;
};
