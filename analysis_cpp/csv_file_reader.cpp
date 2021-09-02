#include "csv_file_reader.hpp"

#include <cstdlib>

// Include header to easily read CSV files:
// (https://github.com/ben-strasser/fast-cpp-csv-parser)
#include "csv.h"
// Define the format of the CSV file
using CsvReader = io::CSVReader<8, io::trim_chars<' '>, io::no_quote_escape<','>>;

// C++20 format library is not yet implemented in most compilers
#if __has_include(<format>)
#include <format>
namespace std_format = std;
#else
// If not use fmt as replacement which has the same implementation for format:
// (https://github.com/fmtlib/fmt)
#include <fmt/core.h>
namespace std_format = fmt;
#endif




ConnectionState getConnectionState(const std::string &connectionState)
{
    if (connectionState == "CONNECTION") [[likely]] {
        return ConnectionState::CONNECTION;
    }
    else if (connectionState == "NO_CONNECTION") {
        return ConnectionState::NO_CONNECTION;
    }
    else if (connectionState == "DNS_LOOKUP_FAILED") {
        return ConnectionState::DNS_LOOKUP_FAILED;
    }
    else [[unlikely]] {
            throw CsvFileReadException(std_format::format("Unknown connection State (connectionState={})",
                                       connectionState));
        }
    }

std::shared_ptr<CsvFileEntries> readCsvFile(const std::filesystem::path &filePath)
{
    // Check if file exists and a regular file
    if (!std::filesystem::exists(filePath)) {
        throw CsvFileReadException(std_format::format("File was not found (filePath={})",
                                   filePath.string()));
    }
    if (!std::filesystem::is_regular_file(filePath)) {
        throw CsvFileReadException(std_format::format("File is not a regular file (filePath={})",
                                   filePath.string()));
    }

    // Use the included class to read the CSV file
    std::unique_ptr<CsvReader> csvCollectionReader;
    try {
        csvCollectionReader = std::make_unique<CsvReader>(filePath);
    }
    catch (const io::error::can_not_open_file &except) {
        throw CsvFileReadException(std_format::format("File could not be opened (filePath={},error={})",
                                   filePath.string(), except.what()));
    }
    csvCollectionReader->read_header(io::ignore_extra_column,
                                     "DATE_ISO", "URL", "IP_ADDRESS", "CONNECTION_ATTEMPTS", "CONNECTION_STATE",
                                     "CONNECTION_SPEED_MS_AVG", "CONNECTION_SPEED_MS_MAX", "CONNECTION_SPEED_MS_MIN");

    // Create container for CSV entries
    auto csvEntries = std::make_shared<CsvFileEntries>();
    // Create variables for each column
    std::string dateIso;
    std::string url;
    std::string ipAddress;
    std::string connectionAttempts;
    std::string connectionState;
    std::string connectionSpeedMsAvg;
    std::string connectionSpeedMsMax;
    std::string connectionSpeedMsMin;

    // Read in the rows
    while (csvCollectionReader->read_row(dateIso, url, ipAddress,
                                         connectionAttempts, connectionState, connectionSpeedMsAvg,
                                         connectionSpeedMsMax, connectionSpeedMsMin)) {
        csvEntries->emplace_back(
            dateIso,
            url,
            ipAddress,
            static_cast<unsigned int>(stoul(connectionAttempts)),
            getConnectionState(connectionState),
            (connectionSpeedMsAvg == "NaN" || connectionSpeedMsAvg.empty())
            ? std::optional<float> {}
            : std::stof(connectionSpeedMsAvg),
            (connectionSpeedMsMax == "NaN" || connectionSpeedMsMax.empty())
            ? std::optional<float> {}
            : std::stof(connectionSpeedMsMax),
            (connectionSpeedMsMin == "NaN" || connectionSpeedMsMin.empty())
            ? std::optional<float> {}
            : std::stof(connectionSpeedMsMin)
        );
    }

    return csvEntries;
}
