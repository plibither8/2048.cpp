#ifndef STATISTICSGRAPHICS_H
#define STATISTICSGRAPHICS_H

#include <string>
#include <tuple>

namespace Statistics {
using total_stats_display_data_t =
    std::tuple<bool, std::string, std::string, std::string, std::string,
               std::string>;
std::string TotalStatisticsOverlay(total_stats_display_data_t tsdd);
} // namespace Statistics

#endif
