#include "statistics.hpp"
#include <fstream>

bool Stats::collectStatistics(int size) {

  std::string file_dir = "../data/statistics" + std::to_string(size) + ".txt";
  std::fstream statistics(file_dir);
  if (statistics.fail()) {
    bestScore = 0;
    gameCount = 0;
    winCount = 0;
    totalMoveCount = 0;
    totalDuration = 0;
    return false;
  }

  statistics >> bestScore >> gameCount >> winCount >> totalMoveCount >>
      totalDuration;
  return true;
}
