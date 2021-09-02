#pragma once

#include <tuple>
#include <string>
#include <map>
#include <memory>
#include <optional>
#include <filesystem>
#include <vector>

enum ConnectionState {
    CONNECTION,
    DNS_LOOKUP_FAILED,
    NO_CONNECTION,
};

struct CsvFileEntry {
    std::string dateIsoString;
    std::string url;
    std::string ipAddressV4;
    unsigned int connectionAttempts;
    ConnectionState connectionState;
    std::optional<float> connectionSpeedMsAvg;
    std::optional<float> connectionSpeedMsMax;
    std::optional<float> connectionSpeedMsMin;
};

struct CsvFileReadException : public std::exception
{
   std::string excpetionMessage;
   CsvFileReadException(const std::string& excpetionMessage) : excpetionMessage(excpetionMessage) {};
   ~CsvFileReadException() throw () {};
   const char* what() const throw() { return excpetionMessage.c_str(); };
};

using CsvFileEntries = std::vector<CsvFileEntry>;

std::shared_ptr<CsvFileEntries> readCsvFile(const std::filesystem::path& filePath);
