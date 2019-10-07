#ifndef SCORESGRAPHICS_H
#define SCORESGRAPHICS_H

#include <string>
#include <vector>

namespace Scoreboard {
namespace Graphics {
using scoreboard_display_data_t =
    std::tuple<std::string, std::string, std::string, std::string, std::string,
               std::string, std::string>;

using scoreboard_display_data_list_t = std::vector<scoreboard_display_data_t>;
std::string ScoreboardOverlay(scoreboard_display_data_list_t sbddl);
} // namespace Graphics
} // namespace Scoreboard

#endif
