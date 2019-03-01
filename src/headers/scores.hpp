#ifndef SCORES_H
#define SCORES_H

#include "global.hpp"
#include <iosfwd>
#include <string>
#include <vector>

struct Score {
  std::string name;
  ull score;
  bool win;
  ull largestTile;
  long long moveCount;
  double duration;
};

bool saveToFileScore(std::string filename, Score s);

std::istream &operator>>(std::istream &is, Score &s);
std::ostream &operator<<(std::ostream &os, Score &s);

class Scoreboard {
private:
  std::vector<Score> scoreList;
  void readFile();

public:
  void printScore();
};
#endif
