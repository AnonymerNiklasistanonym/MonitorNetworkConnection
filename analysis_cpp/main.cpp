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

#include "csv_file_reader.hpp"
#include "plot_graph.hpp"
#include "time_converter.hpp"

//! Helper function to measure time difference.
template <typename U = std::chrono::milliseconds, typename T>
inline auto getTimeDifference(const std::chrono::time_point<T> &startTimePoint,
                              const std::chrono::time_point<T> &endTimePoint)
{
    return std::chrono::duration_cast<U>(endTimePoint - startTimePoint).count();
}

std::time_t parseIsoDateStringToGnuPlotTime2(const std::string &isoDateString)
{
    struct tm tm;
    strptime(isoDateString.c_str(), "%FT%T%z", &tm);
    return std::mktime(&tm);
}

std::string parseDateToGnuPlotString2(const time_t &timeObject)
{
    tm localTimeObject = *localtime(&timeObject);
    std::stringstream ss;
    ss << localTimeObject.tm_mday << "/" << localTimeObject.tm_mon << "/" << localTimeObject.tm_year;
    return ss.str();
}

#include <string.h>
#include <stdio.h>
#include <time.h>


int main()
{
    const auto dataPath = std::filesystem::path("..")  / ".." / "data" / "data.csv";

    std::cout << std_format::format("Read CSV file: '{}'", dataPath.string()) << std::endl;

    try {
        const auto readCsvFileTimeBegin = std::chrono::high_resolution_clock::now();
        const auto csvFileEntries = readCsvFile(dataPath);
        const auto readCsvFileTimeEnd = std::chrono::high_resolution_clock::now();

        const auto timeDifferenceInMs = getTimeDifference(readCsvFileTimeBegin, readCsvFileTimeEnd);
        std::cout << std_format::format("Time to read CSV file: {}ms ({}s)",
                                        timeDifferenceInMs, timeDifferenceInMs / 1000) << std::endl;

        std::size_t count = 0;
        for (auto const &csvFileEntry : *csvFileEntries) {
            // Check if element has no connection
            if (csvFileEntry.connectionState != ConnectionState::CONNECTION ||
                !csvFileEntry.connectionSpeedMsAvg.has_value()) {
                count++;
                // std::cout << std_format::format("new: Found NO_CONNECTION at {} for {}\n",
                //     csvFileEntry.dateIsoString, csvFileEntry.url);
            }
        }

        std::cout << std_format::format("Found {} elements with no connection or other problems",
                                        count) << std::endl;

        //std::time_t from;
        //std::ctime(&from);
        //std::time_t to;
        //std::ctime(&to);

        const time_t from = csvFileEntries->at(0).dateTimeT;
        //const time_t from = convertIso8601DateStringToLocalTimeT("2021-09-14T00:00:00.000Z");
        const time_t to = csvFileEntries->at(csvFileEntries->size() - 1).dateTimeT;
        //const time_t to = convertIso8601DateStringToLocalTimeT("2021-09-15T00:00:00.000Z");
        const std::optional<float> maxValue = { 1000 };
        const std::optional<std::string> specificUrl = {};
        const auto gnuplotPathPng = std::filesystem::path("..")  / ".." / "data" / "gnuplot.png";
        const GnuPlotOptions gnuPlotOptionsPng = {
            .title = "Avg connection speed time",
            .size = { 1920 * 16, 1080 * 2 },
            .outputType = GnuPlotOutputType::PNG_FILE,
        };
        std::cout << std_format::format("Plot a GnuPlot graph from {} to {} and save it in {}",
                                        convertLocalTimeTToIso8601DateString(from),
                                        convertLocalTimeTToIso8601DateString(to),
                                        gnuplotPathPng.string()) << std::endl;
        const auto plotGraphTimeBegin = std::chrono::high_resolution_clock::now();
        plotAvgConnectionSpeedTimelineGnuPlot(csvFileEntries, from, to, gnuplotPathPng, gnuPlotOptionsPng,
                                              specificUrl, maxValue);
        const auto plotGraphTimeEnd = std::chrono::high_resolution_clock::now();

        const auto plotGraphTimeDifferenceInMs = getTimeDifference(plotGraphTimeBegin, plotGraphTimeEnd);
        std::cout << std_format::format("Time to plot graph: {}ms ({}s)",
                                        plotGraphTimeDifferenceInMs, plotGraphTimeDifferenceInMs / 1000) << std::endl;

        // Rendering many data points in a PDF takes to long to be useful but can be cool
        // if there are not that many
        //const auto gnuplotPathPdf = std::filesystem::path("..")  / ".." / "data" / "gnuplot.pdf";
        //const GnuPlotOptions gnuPlotOptionsPdf = {
        //    .title = "Avg connection speed time",
        //    .size = { 1920 * 16, 1080 * 2 },
        //    .outputType = GnuPlotOutputType::PDF_FILE,
        //};
        //plotAvgConnectionSpeedTimelineGnuPlot(csvFileEntries, from, to, gnuplotPathPdf, gnuPlotOptionsPdf,
        //                                      specificUrl, maxValue);
    }
    catch (const CsvFileReadException &e) {
        std::cerr << std_format::format("Exception was thrown while reading the CSV file: {}",
                                        e.what()) << std::endl;
        return EXIT_FAILURE;
    }
    catch (const std::exception &e) {
        std::cerr << std_format::format("Unknown exception was thrown: {}",
                                        e.what()) << std::endl;
        return EXIT_FAILURE;
    }
}
