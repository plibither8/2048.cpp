#ifndef SCORES_H
#define SCORES_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <tuple>
#include <vector>

namespace Scoreboard {
struct Score {
  std::string name;
  ull score;
  bool win;
  ull largestTile;
  long long moveCount;
  double duration;
};

bool operator>(const Score &a, const Score &b);

using Scoreboard_t = std::vector<Score>;
using load_score_status_t = std::tuple<bool, Scoreboard_t>;

// List of scores read until "exhausted".
// Note: returns a tuple containing a std::vector<Score> of all read scores.
load_score_status_t loadFromFileScore(std::string filename);
void saveScore(Score finalscore);
} // namespace Scoreboard

std::istream &operator>>(std::istream &is, Scoreboard::Score &s);
std::ostream &operator<<(std::ostream &os, Scoreboard::Score &s);

#endif
