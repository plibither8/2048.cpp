#include "statistics.hpp"
#include <fstream>

namespace {

Stats generateStatsFromInputData(std::istream &is) {
  Stats stats;
  is >> stats;
  return stats;
}

bool generateFilefromStatsData(std::ostream &os, Stats stats) {
  os << stats;
  return true;
}

} // namespace

load_stats_status_t loadFromFileStatistics(std::string filename) {
  std::ifstream statistics(filename);
  if (statistics) {
    Stats stats = generateStatsFromInputData(statistics);
    return load_stats_status_t{true, stats};
  }
  return load_stats_status_t{false, Stats{}};
}

bool saveToFileStatistics(std::string filename, Stats s) {
  std::ofstream filedata(filename);
  return generateFilefromStatsData(filedata, s);
}

std::istream &operator>>(std::istream &is, Stats &s) {
  is >> s.bestScore >> s.gameCount >> s.winCount >> s.totalMoveCount >>
      s.totalDuration;
  return is;
}

std::ostream &operator<<(std::ostream &os, Stats &s) {
  os << s.bestScore << "\n"
     << s.gameCount << "\n"
     << s.winCount << "\n"
     << s.totalMoveCount << "\n"
     << s.totalDuration;
  return os;
}
