#include "statistics.hpp"
#include "color.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <sstream>

namespace Statistics {

namespace {

total_game_stats_t generateStatsFromInputData(std::istream &is) {
  total_game_stats_t stats;
  is >> stats;
  return stats;
}

bool generateFilefromStatsData(std::ostream &os, total_game_stats_t stats) {
  os << stats;
  return true;
}

} // namespace

load_stats_status_t loadFromFileStatistics(std::string filename) {
  std::ifstream statistics(filename);
  if (statistics) {
    total_game_stats_t stats = generateStatsFromInputData(statistics);
    return load_stats_status_t{true, stats};
  }
  return load_stats_status_t{false, total_game_stats_t{}};
}

bool saveToFileEndGameStatistics(std::string filename, total_game_stats_t s) {
  std::ofstream filedata(filename);
  return generateFilefromStatsData(filedata, s);
}

void prettyPrintStats(std::ostream &os) {
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
    std::for_each(std::begin(stats_attributes_text),
                  std::end(stats_attributes_text), populate_stats_info);
    stats_richtext << sp << footer_border_text << "\n";

  } else {
    stats_richtext << sp << no_save_text << "\n";
  }

  stats_richtext << "\n\n\n";
  stats_richtext << sp << any_key_exit_text;

  os << stats_richtext.str();
}

} // namespace Statistics

using namespace Statistics;

std::istream &operator>>(std::istream &is, total_game_stats_t &s) {
  is >> s.bestScore >> s.gameCount >> s.winCount >> s.totalMoveCount >>
      s.totalDuration;
  return is;
}

std::ostream &operator<<(std::ostream &os, total_game_stats_t &s) {
  os << s.bestScore << "\n"
     << s.gameCount << "\n"
     << s.winCount << "\n"
     << s.totalMoveCount << "\n"
     << s.totalDuration;
  return os;
}
