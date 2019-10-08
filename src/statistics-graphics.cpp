#include "statistics-graphics.hpp"
#include "color.hpp"
#include "statistics.hpp"
#include <array>
#include <iomanip>
#include <sstream>

namespace Statistics {

std::string TotalStatisticsOverlay() {
  constexpr auto stats_title_text = "STATISTICS";
  constexpr auto divider_text = "──────────";
  constexpr auto header_border_text = "┌────────────────────┬─────────────┐";
  constexpr auto footer_border_text = "└────────────────────┴─────────────┘";
  const auto stats_attributes_text = {"Best Score", "Game Count",
                                      "Number of Wins", "Total Moves Played",
                                      "Total Duration"};
  constexpr auto no_save_text = "No saved statistics.";
  constexpr auto any_key_exit_text =
      "Press any key to return to the main menu... ";
  constexpr auto sp = "  ";

  std::ostringstream stats_richtext;

  total_game_stats_t stats;
  bool stats_file_loaded{};
  std::tie(stats_file_loaded, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  if (stats_file_loaded) {
    constexpr auto num_of_stats_attributes_text = 5;
    auto data_stats = std::array<std::string, num_of_stats_attributes_text>{};
    data_stats = {
        std::to_string(stats.bestScore), std::to_string(stats.gameCount),
        std::to_string(stats.winCount), std::to_string(stats.totalMoveCount),
        secondsFormat(stats.totalDuration)};

    auto counter{0};
    const auto populate_stats_info = [data_stats, stats_attributes_text,
                                      &counter,
                                      &stats_richtext](const std::string) {
      stats_richtext << sp << "│ " << bold_on << std::left << std::setw(18)
                     << std::begin(stats_attributes_text)[counter] << bold_off
                     << " │ " << std::right << std::setw(11)
                     << data_stats[counter] << " │"
                     << "\n";
      counter++;
    };

    stats_richtext << green << bold_on << sp << stats_title_text << bold_off
                   << def << "\n";
    stats_richtext << green << bold_on << sp << divider_text << bold_off << def
                   << "\n";
    stats_richtext << sp << header_border_text << "\n";

    for (const auto s : stats_attributes_text) {
      populate_stats_info(s);
    }
    stats_richtext << sp << footer_border_text << "\n";

  } else {
    stats_richtext << sp << no_save_text << "\n";
  }

  stats_richtext << "\n\n\n";
  stats_richtext << sp << any_key_exit_text;

  return stats_richtext.str();
}

} // namespace Statistics
