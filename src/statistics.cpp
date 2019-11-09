#include "statistics.hpp"
#include "color.hpp"
#include <algorithm>
#include <array>
#include <fstream>
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

ull load_game_best_score() {
  total_game_stats_t stats;
  bool stats_file_loaded{};
  ull tempscore{0};
  std::tie(stats_file_loaded, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  if (stats_file_loaded) {
    tempscore = stats.bestScore;
  }
  return tempscore;
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
