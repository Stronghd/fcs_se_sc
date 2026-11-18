#include "FileResultExporter.hpp"

#include <filesystem>
#include <fstream>
#include <iomanip>
#include <locale>
#include <stdexcept>

namespace
{
void writeDelimited(std::ostream& output, const ResultReport& report, char separator)
{
    const auto& student = report.student;
    output << "student_id" << separator << "name" << separator << "surname"
           << separator << "result_percent\r\n";
    output << student.getId() << separator << student.getName() << separator
           << student.getSurname() << separator << std::fixed << std::setprecision(2)
           << report.grade.percentage << "\r\n";
}

class CsvFormatter final : public ResultFormatter
{
public:
    void write(std::ostream& output, const ResultReport& report) const override
    {
        writeDelimited(output, report, ',');
    }
};

class TsvFormatter final : public ResultFormatter
{
public:
    void write(std::ostream& output, const ResultReport& report) const override
    {
        writeDelimited(output, report, '\t');
    }
};
}

void FileResultExporter::write(const std::string& path, const ResultReport& report) const
{
    const auto formatter = createFormatter();
    std::ofstream output(std::filesystem::u8path(path), std::ios::binary);
    if (!output)
    {
        throw std::runtime_error("Не удалось открыть файл для экспорта");
    }

    output.imbue(std::locale::classic());
    formatter->write(output, report);
    output.close();
    if (!output)
    {
        throw std::runtime_error("Не удалось полностью записать файл");
    }
}

std::unique_ptr<ResultFormatter> CsvResultExporter::createFormatter() const
{
    return std::make_unique<CsvFormatter>();
}

std::unique_ptr<ResultFormatter> TsvResultExporter::createFormatter() const
{
    return std::make_unique<TsvFormatter>();
}
