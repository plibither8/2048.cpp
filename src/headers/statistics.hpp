#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>
#include <string>
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

load_stats_status_t loadFromFileStatistics(std::string filename);
bool saveToFileStatistics(std::string filename, Stats s);

#endif
