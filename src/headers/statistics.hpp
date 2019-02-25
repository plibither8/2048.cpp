#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>
#include <tuple>

struct Stats {
  ull bestScore{};
  ull totalMoveCount{};
  int gameCount{};
  double totalDuration{};
  int winCount{};
};

using load_stats_status_t = std::tuple<bool, Stats>;

std::istream &operator>>(std::istream &is, Stats &s);
std::ostream &operator<<(std::ostream &os, Stats &s);

load_stats_status_t loadFromFileStatistics();

#endif
