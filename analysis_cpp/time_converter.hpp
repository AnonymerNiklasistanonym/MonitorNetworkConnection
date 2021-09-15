#pragma once

#include <ctime>
#include <string>
#include <string_view>

std::time_t convertIso8601DateStringToLocalTimeT(const std::string_view &time_string);
std::string convertLocalTimeTToIso8601DateString(const std::time_t &time_t);
