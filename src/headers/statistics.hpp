#ifndef STATISTICS_H
#define STATISTICS_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <tuple>

namespace Scoreboard {
struct Score;
}

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
ull load_game_best_score();
void saveEndGameStats(Scoreboard::Score finalscore);
void CreateFinalScoreAndEndGameDataFile(std::ostream &os, std::istream &is,
                                        Scoreboard::Score finalscore);
} // namespace Statistics

std::istream &operator>>(std::istream &is, Statistics::total_game_stats_t &s);
std::ostream &operator<<(std::ostream &os, Statistics::total_game_stats_t &s);

#endif
