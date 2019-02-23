#include "statistics.hpp"
#include <fstream>

bool collectStatistics(Stats &stats) {

  std::ifstream statistics("../data/statistics.txt");
  if (statistics.fail()) {
    return false;
  }

  statistics >> stats;
  return true;
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
