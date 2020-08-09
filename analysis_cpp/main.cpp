#include <iostream>
#include <filesystem>
#include <chrono>
#include <memory>
#include <map>
#include <tuple>

// C++20 format library is not yet implemented in most compilers
#if __has_include(<format>)
#include <format>
namespace format = std;
#else
// If not use fmt as replacement which has the same implementation for format:
// (https://github.com/fmtlib/fmt)
#include <fmt/core.h>
namespace format = fmt;
#endif

// Include header to easily read CSV files:
// (https://github.com/ben-strasser/fast-cpp-csv-parser)
#include "csv.h"


typedef std::tuple<std::string, std::string, std::string, std::string, std::string, std::string, std::string> dataPtrData;
typedef std::shared_ptr<dataPtrData> dataPtr;
typedef std::map<std::string, dataPtr> collectionPtrData;
typedef std::shared_ptr<collectionPtrData> collectionPtr;

typedef io::CSVReader<8, io::trim_chars<' '>, io::no_quote_escape<','>> csvReader;


//! Helper function to measure time difference.
template <typename U = std::chrono::milliseconds, typename T>
inline auto getTimeDifference(const std::chrono::time_point<T> &startTimePoint,
                              const std::chrono::time_point<T> &endTimePoint)
{
    return std::chrono::duration_cast<U>(endTimePoint - startTimePoint).count();
}


int main () {
    const auto dataPath = std::filesystem::path("..")  / ".." / "data" / "data.csv";

    if (!std::filesystem::exists(dataPath) || !std::filesystem::is_regular_file(dataPath)) {
        std::cerr << format::format("Data text file was not found: ' {}'", dataPath.string()) << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << format::format("Data text file was found: ' {}'", dataPath.string()) << std::endl;

    // Measure time
    auto beginCollectData = std::chrono::high_resolution_clock::now();

    // Open CSV file and read header
    std::unique_ptr<csvReader> csvCollectionReader;
    try
    {
        csvCollectionReader = std::make_unique<csvReader>(dataPath.string());
    }
    catch (const io::error::can_not_open_file &except)
    {
        std::cerr << format::format("The file \"{}\" could not be opened.\nError message: {}",
            except.file_name, except.what());
        return EXIT_FAILURE;
    }
    csvCollectionReader->read_header(io::ignore_extra_column,
        "DATE_ISO","URL","IP_ADDRESS", "CONNECTION_ATTEMPTS","CONNECTION_STATE",
        "CONNECTION_SPEED_MS_AVG", "CONNECTION_SPEED_MS_MAX","CONNECTION_SPEED_MS_MIN");
    // Create variables for each column of each row
    std::string dateIso;
    std::string url;
    std::string ipAddress;
    std::string connectionAttempts;
    std::string connectionState;
    std::string connectionSpeedMsAvg;
    std::string connectionSpeedMsMax;
    std::string connectionSpeedMsMin;
    // Create variables for collection, documents
    collectionPtr collection = std::make_shared<collectionPtrData>();
    // Read in documents
    while (csvCollectionReader->read_row(dateIso, url, ipAddress,
        connectionAttempts, connectionState, connectionSpeedMsAvg,
        connectionSpeedMsMax, connectionSpeedMsMin))
    {
        collection->emplace(dateIso, std::make_shared<dataPtrData>(std::tuple{
            url, ipAddress, connectionAttempts, connectionState,
            connectionSpeedMsAvg, connectionSpeedMsMax, connectionSpeedMsMin
        }));
    }

    // Measure time
    auto endCollectData = std::chrono::high_resolution_clock::now();
    std::cout << format::format("Execution time (data collection): {} ms",
        getTimeDifference(beginCollectData, endCollectData)) << std::endl;

    std::cout << format::format("{} elements were read", collection->size()) << std::endl;

    std::size_t count = 0;
    for( auto const& [key, val] : *collection )
    {
        // Check if element has no connection
        if (std::get<3>(*val) == "NO_CONNECTION") {
            count++;
            std::cout << format::format("Found NO_CONNECTION at {} for {}\n",
                key, std::get<0>(*val));
        }
    }
    std::flush(std::cout);
    std::cout << format::format("Found {} elements with no connection", count) << std::endl;
}
