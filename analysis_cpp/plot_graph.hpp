#pragma once

#include <ctime>
#include <filesystem>

#include "data_point.hpp"

// TODO Implement

void plotAvgConnectionSpeedTimelineMatplotLib(const DataPointCollection &dataCollection,
        const time_t &from, const time_t &to, const std::filesystem::path &filePath,
        const std::optional<std::string> &url);

void plotConnectionSpeedTimelineMatplotLib(const DataPointCollection &dataCollection,
        const time_t &from, const time_t &to, const std::string &url,
        const std::filesystem::path &filePath);

void plotAvgConnectionSpeedTimelineGnuPlot(const DataPointCollection &dataCollection,
        const time_t &from, const time_t &to, const std::filesystem::path &filePath,
        const std::optional<std::string> &url);

void plotConnectionSpeedTimelineGnuPlot(const DataPointCollection &dataCollection,
                                        const time_t &from, const time_t &to, const std::string &url,
                                        const std::filesystem::path &filePath);
