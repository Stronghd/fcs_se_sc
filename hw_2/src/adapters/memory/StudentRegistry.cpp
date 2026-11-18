#include "StudentRegistry.hpp"

std::uint64_t StudentRegistry::addStudent(std::string name, std::string surname)
{
    const std::uint64_t id = nextId_;
    students_.emplace(id, Student(id, name, surname));
    ++nextId_;
    return id;
}

const Student *StudentRegistry::findStudent(const std::uint64_t &id) const
{
    const auto answer = students_.find(id);
    if (answer != students_.end())
        return &(answer->second);
    else
        return nullptr;
}

std::vector<Student> StudentRegistry::getStudents() const
{
    std::vector<Student> answer;
    for (auto &student : students_)
    {
        answer.push_back(student.second);
    }
    return answer;
}