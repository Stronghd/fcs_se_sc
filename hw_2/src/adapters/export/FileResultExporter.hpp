#pragma once

#include <iosfwd>
#include <memory>

#include "ResultExporter.hpp"

class ResultFormatter
{
public:
    virtual ~ResultFormatter() = default;
    virtual void write(std::ostream& output, const ResultReport& report) const = 0;
};

class FileResultExporter : public ResultExporter
{
public:
    void write(const std::string& path, const ResultReport& report) const override;

protected:
    virtual std::unique_ptr<ResultFormatter> createFormatter() const = 0;
};

class CsvResultExporter final : public FileResultExporter
{
protected:
    std::unique_ptr<ResultFormatter> createFormatter() const override;
};

class TsvResultExporter final : public FileResultExporter
{
protected:
    std::unique_ptr<ResultFormatter> createFormatter() const override;
};
