#include "time_converter.hpp"

#include <cstring>

std::time_t convertIso8601DateStringToLocalTimeT(const std::string_view &time_string)
{
    tzset();

    struct tm ctime;
    std::memset(&ctime, 0, sizeof(struct tm));
    strptime(time_string.data(), "%FT%T%z", &ctime);

    std::time_t ts = mktime(&ctime) - timezone;
    return ts;
}

#include <iomanip>
#include <ctime>
#include <sstream>

std::wstring wtime(const time_t &t)
{
    std::tm tm = *std::localtime(&t);
    std::wstringstream wss;
    wss << std::put_time(&tm, L"%F %T");
    return wss.str();
}

std::string convertLocalTimeTToIso8601DateString(const std::time_t &time_t)
{
    const auto wtimeString = wtime(time_t);
    std::string str(wtimeString.begin(), wtimeString.end());
    return str;
}
