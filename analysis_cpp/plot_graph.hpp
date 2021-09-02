#pragma once

#include <ctime>
#include <filesystem>

#include "data_point.hpp"

// TODO Implement

void plotAvgConnectionSpeedTimelineMatplotLib(const DataPointCollection &dataCollection,
        const std::time_t &from, const std::time_t &to, const std::filesystem::path &filePath,
        const std::optional<std::string> &url);

void plotConnectionSpeedTimelineMatplotLib(const DataPointCollection &dataCollection,
        const std::time_t &from, const std::time_t &to, const std::string &url,
        const std::filesystem::path &filePath);

enum GnuPlotOutputType {
    INTERACTIVE_WINDOW,
    PDF_FILE,
    PNG_FILE,
    TRANSPARENT_PNG_FILE,
};

struct GnuPlotOptions {
    std::string title;
    std::tuple<unsigned int, unsigned int> size;
    std::optional<float> fontSize = {};
    std::optional<std::string> fontFamily = {};
    std::optional<float> fontScale = {};
    std::optional<float> fontSizeTitle = {};
    std::optional<GnuPlotOutputType> outputType = {};
};

void plotAvgConnectionSpeedTimelineGnuPlot(const DataPointCollection &dataCollection,
        const std::time_t &from, const std::time_t &to, const std::filesystem::path &filePath,
        const GnuPlotOptions &gnuPlotOptions, const std::optional<std::string> &url,
        const std::optional<float> &maxAvgConnectionSpeedToDisplay);

void plotConnectionSpeedTimelineGnuPlot(const DataPointCollection &dataCollection,
                                        const std::time_t &from, const std::time_t &to, const std::string &url);
