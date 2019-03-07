#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <tuple>

namespace Statistics {
struct total_game_stats_t {
  ull bestScore{};
  ull totalMoveCount{};
  int gameCount{};
  double totalDuration{};
  int winCount{};
};

using load_stats_status_t = std::tuple<bool, total_game_stats_t>;

load_stats_status_t loadFromFileStatistics(std::string filename);
bool saveToFileEndGameStatistics(std::string filename, total_game_stats_t s);
void prettyPrintStats(std::ostream &os);
} // namespace Statistics

std::istream &operator>>(std::istream &is, Statistics::total_game_stats_t &s);
std::ostream &operator<<(std::ostream &os, Statistics::total_game_stats_t &s);

#endif
