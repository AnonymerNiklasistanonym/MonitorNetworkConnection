#include "plot_graph.hpp"

#include <set>
#include <sstream>
#include <algorithm>

// Warn about use of deprecated gnuplot functions.
#define GNUPLOT_DEPRECATE_WARN
#include "gnuplot-iostream.h"

void demo_basic()
{
    Gnuplot gp;
    // For debugging or manual editing of commands:
    //Gnuplot gp(std::fopen("plot.gnu"));
    // or
    //Gnuplot gp("tee plot.gnu | gnuplot -persist");

    std::vector<std::pair<double, double>> xy_pts_A;
    for (double x = -2; x < 2; x += 0.01) {
        double y = x * x * x;
        xy_pts_A.push_back(std::make_pair(x, y));
    }

    std::vector<std::pair<double, double>> xy_pts_B;
    for (double alpha = 0; alpha < 1; alpha += 1.0 / 24.0) {
        double theta = alpha * 2.0 * 3.14159;
        xy_pts_B.push_back(std::make_pair(cos(theta), sin(theta)));
    }

    gp << "set xrange [-2:2]\nset yrange [-2:2]\n";
    gp << "plot '-' with lines title 'cubic', '-' with points title 'circle'\n";
    gp.send1d(xy_pts_A);
    gp.send1d(xy_pts_B);
}

std::string parsTimeTToGnuPlotString(const time_t &timeObject)
{
    tm localTimeObject = *localtime(&timeObject);
    std::stringstream ss;
    ss << localTimeObject.tm_mday << "/" << localTimeObject.tm_mon << "/" << localTimeObject.tm_year;
    return ss.str();
}

//std::time_t convertIso8601TimeT(const std::string time_string)
//{
//    tzset();
//
//    struct tm ctime;
//    memset(&ctime, 0, sizeof(struct tm));
//    strptime(time_string.c_str(), "%FT%T%z", &ctime);
//
//    std::time_t ts = mktime(&ctime) - timezone;
//    return ts;
//}
//
//tm convertIso8601Tm(const std::string time_string)
//{
//    auto ts = convertIso8601TimeT(time_string);
//    struct tm tm_data;
//    localtime_r(&ts, &tm_data);
//    return tm_data;
//}

#include "time_converter.hpp"

std::string convertTimeTToGnuPlotTimeString(const std::time_t &time_t)
{
    return std::string("\"") + convertLocalTimeTToIso8601DateString(time_t) + std::string("\"");
}

void plotAvgConnectionSpeedTimelineGnuPlot(const DataPointCollection &dataCollection,
        const std::time_t &from, const std::time_t &to, const std::filesystem::path &filePath,
        const GnuPlotOptions &gnuPlotOptions, const std::optional<std::string> &url,
        const std::optional<float> &maxAvgConnectionSpeedToDisplay)
{
    // demo_basic();

    std::set<std::string> urls;
    for (auto const &dataPoint : *dataCollection) {
        if (!url.has_value() || dataPoint.url == url.value()) {
            urls.insert(dataPoint.url);
        }
    }

    Gnuplot gp;

    if (gnuPlotOptions.outputType.has_value()) {
        switch (gnuPlotOptions.outputType.value()) {
            case GnuPlotOutputType::TRANSPARENT_PNG_FILE:
                gp << "set terminal pngcairo transparent enhanced font \"";
                gp << gnuPlotOptions.fontFamily.value_or("arial") << "," << gnuPlotOptions.fontSize.value_or(
                       10) << "\" fontscale " << gnuPlotOptions.fontScale.value_or(1.0) << " size " <<
                   std::get<0>(gnuPlotOptions.size) << "," << std::get<1>(gnuPlotOptions.size) << "\n";
                break;
            case GnuPlotOutputType::PNG_FILE:
                gp << "set terminal pngcairo enhanced font \"";
                gp << gnuPlotOptions.fontFamily.value_or("arial") << "," << gnuPlotOptions.fontSize.value_or(
                       10) << "\" fontscale " << gnuPlotOptions.fontScale.value_or(1.0) << " size " <<
                   std::get<0>(gnuPlotOptions.size) << "," << std::get<1>(gnuPlotOptions.size) << "\n";
                break;
            case GnuPlotOutputType::PDF_FILE:
                gp << "set terminal pdf enhanced font \"";
                gp << gnuPlotOptions.fontFamily.value_or("arial") << "," << gnuPlotOptions.fontSize.value_or(
                       10) << "\" fontscale " << gnuPlotOptions.fontScale.value_or(1.0) << " size " <<
                   std::get<0>(gnuPlotOptions.size) << "," << std::get<1>(gnuPlotOptions.size) << "\n";
                break;
            default:
                break;
        }
        switch (gnuPlotOptions.outputType.value()) {
            case GnuPlotOutputType::TRANSPARENT_PNG_FILE:
            case GnuPlotOutputType::PNG_FILE:
            case GnuPlotOutputType::PDF_FILE:
                gp << "set output '" << filePath.string() << "'\n";
                break;
            default:
                break;
        }
    }

    gp << "set title \"" << gnuPlotOptions.title << "\" font \"," <<
       gnuPlotOptions.fontSizeTitle.value_or(14) << "\"\n";
    gp << "set xlabel 'Date'\n";
    gp << "set xrange [ \"" << from << "\":\"" << to << "\" ]\n";
    //gp << "set xdata time\n";
    //gp << "set timefmt '\"%Y-%m-%d %H:%M:%S\"'\n";
    //gp << "set set xtics format '%Y-%m-%d %H:%M'\n";
    //gp << "set xrange [ \"" << convertTimeTToGnuPlotTimeString(from) << "\":\"" <<
    //   convertTimeTToGnuPlotTimeString(to) << "\" ]\n";
    gp << "set ylabel 'Average Ping (N=5)'\n";
    gp << "set yrange [ 0 : ";
    if (maxAvgConnectionSpeedToDisplay.has_value()) {
        gp << maxAvgConnectionSpeedToDisplay.value() << " ";
    }
    gp << "]\n";
    gp << "plot ";
    for (auto const &url : urls) {
        gp << "'-' with lines title '" << url << "', ";
    }
    gp << "\n";
    //gp << "set xtics (";
    //std::time_t lastDate = 0;
    //for (auto const &dataPoint : *dataCollection) {
    //    if (lastDate + (24 * 60 * 60) < dataPoint.dateTimeT) {
    //        if (lastDate != 0) {
    //            gp << ", ";
    //        }
    //        lastDate = dataPoint.dateTimeT;
    //        gp << "'" << convertTimeTToGnuPlotTimeString(lastDate) << "' " << lastDate;
    //    }
    //
    //}
    //gp << ")\n";

    for (auto const &url : urls) {
        std::vector<std::pair<std::time_t, float>> xy_pts;
        for (auto const &dataPoint : *dataCollection) {
            if (dataPoint.url == url && dataPoint.dateTimeT < to && dataPoint.dateTimeT > from) {
                xy_pts.emplace_back(/*convertTimeTToGnuPlotTimeString(*/dataPoint.dateTimeT/*)*/,
                        dataPoint.connectionSpeedMsAvg.value_or(0));
            }
        }
        gp.send1d(xy_pts);
    }
}
