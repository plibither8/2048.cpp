#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <tuple>

struct total_game_stats_t {
  ull bestScore{};
  ull totalMoveCount{};
  int gameCount{};
  double totalDuration{};
  int winCount{};
};

using load_stats_status_t = std::tuple<bool, total_game_stats_t>;

std::istream &operator>>(std::istream &is, total_game_stats_t &s);
std::ostream &operator<<(std::ostream &os, total_game_stats_t &s);

load_stats_status_t loadFromFileStatistics(std::string filename);
bool saveToFileStatistics(std::string filename, total_game_stats_t s);

#endif
