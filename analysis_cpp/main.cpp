#include <iostream>
#include <filesystem>
#include <chrono>

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

// Include header to easily read CSV files:
// (https://github.com/ben-strasser/fast-cpp-csv-parser)
#include "csv.h"

#include "date/date.h"

#include "csv_file_reader.hpp"

//! Helper function to measure time difference.
template <typename U = std::chrono::milliseconds, typename T>
inline auto getTimeDifference(const std::chrono::time_point<T> &startTimePoint,
                              const std::chrono::time_point<T> &endTimePoint)
{
    return std::chrono::duration_cast<U>(endTimePoint - startTimePoint).count();
}


int main () {
    const auto dataPath = std::filesystem::path("..")  / ".." / "data" / "data.csv";

    auto readCsvFileTimeBegin = std::chrono::high_resolution_clock::now();
    auto csvFileEntries = readCsvFile(dataPath);
    auto readCsvFileTimeEnd = std::chrono::high_resolution_clock::now();

    std::cout << std_format::format("Time to read CSV file: {} ms",
        getTimeDifference(readCsvFileTimeBegin, readCsvFileTimeEnd)) << std::endl;

    std::size_t count = 0;
    for( auto const& csvFileEntry : *csvFileEntries )
    {
        // Check if element has no connection
        if (csvFileEntry.connectionState != ConnectionState::CONNECTION || !csvFileEntry.connectionSpeedMsAvg.has_value()) {
            count++;
            // std::cout << std_format::format("new: Found NO_CONNECTION at {} for {}\n",
            //     csvFileEntry.dateIsoString, csvFileEntry.url);
        }
    }

    std::cout << std_format::format("Found {} elements with no connection or other problems", count) << std::endl;

}
