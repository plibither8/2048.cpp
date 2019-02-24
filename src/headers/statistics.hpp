#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>

struct Stats {
  ull bestScore{};
  ull totalMoveCount{};
  int gameCount{};
  double totalDuration{};
  int winCount{};
};

std::istream &operator>>(std::istream &is, Stats &s);
std::ostream &operator<<(std::ostream &os, Stats &s);

bool collectStatistics(Stats &stats);

#endif
