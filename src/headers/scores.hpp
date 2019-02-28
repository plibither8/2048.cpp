#ifndef SCORES_H
#define SCORES_H

#include "global.hpp"
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

class Scoreboard {
private:
  std::string name;
  std::vector<Score> scoreList;
  void prompt();
  void writeToFile();
  void readFile();
  void padding(std::string name);

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
