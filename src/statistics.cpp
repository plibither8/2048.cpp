#include "statistics.hpp"
#include <fstream>

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

bool saveToFileStatistics(std::string filename, total_game_stats_t s) {
  std::ofstream filedata(filename);
  return generateFilefromStatsData(filedata, s);
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
