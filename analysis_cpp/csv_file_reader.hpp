#pragma once

#include <exception>
#include <filesystem>

#include "data_point.hpp"

struct CsvFileReadException : public std::exception {
    std::string excpetionMessage;
    CsvFileReadException(const std::string &excpetionMessage) : excpetionMessage(excpetionMessage) {};
    ~CsvFileReadException() throw () {};
    const char *what() const throw() { return excpetionMessage.c_str(); };
};

DataPointCollection readCsvFile(const std::filesystem::path &filePath);
