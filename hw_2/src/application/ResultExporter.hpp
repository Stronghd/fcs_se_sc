#pragma once

#include <string>

#include "FinalGradeService.hpp"
#include "Student.hpp"

struct ResultReport
{
    Student student;
    FinalGrade grade;
};

class ResultExporter
{
public:
    virtual ~ResultExporter() = default;
    virtual void write(const std::string& path, const ResultReport& report) const = 0;
};
