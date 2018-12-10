#ifndef SCORES_H
#define SCORES_H

#include "global.hpp"
#include "menu.hpp"
#include "statistics.hpp"
#include <algorithm>
#include <fstream>
#include <iostream>
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
  void readFile(int size);
  void padding(std::string name);

public:
  ull playsize{0};
  ull score = 0;
  bool win;
  ull largestTile;
  long long moveCount;
  double duration;
  void printScore(int boardsize);
  void printStats(int boardsize);
  void save();
};
#endif
