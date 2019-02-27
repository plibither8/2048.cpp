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

std::istream &operator>>(std::istream &is, Score &s);
std::ostream &operator<<(std::ostream &os, Score &s);

class Scoreboard {
private:
  std::string name;
  std::vector<Score> scoreList;
  void prompt();
  void writeToFile();
  void readFile();

public:
  ull score = 0;
  bool win;
  ull largestTile;
  long long moveCount;
  double duration;
  void printScore();
  void save();
};
#endif
