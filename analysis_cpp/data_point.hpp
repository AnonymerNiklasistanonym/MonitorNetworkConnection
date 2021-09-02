#pragma once

#include <string>
#include <optional>
#include <vector>
#include <memory>

enum ConnectionState {
    CONNECTION,
    DNS_LOOKUP_FAILED,
    NO_CONNECTION,
};

struct DataPoint {
    std::string dateIsoString;
    std::string url;
    std::string ipAddressV4;
    unsigned int connectionAttempts;
    ConnectionState connectionState;
    std::optional<float> connectionSpeedMsAvg;
    std::optional<float> connectionSpeedMsMax;
    std::optional<float> connectionSpeedMsMin;
};

using DataPoints = std::vector<DataPoint>;
using DataPointCollection = std::shared_ptr<DataPoints>;
