#ifndef GAME_H
#define GAME_H

#include "color.hpp"
#include "global.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <chrono>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

enum Directions { UP, DOWN, RIGHT, LEFT };

class Tile {

public:
  Tile() : value(0), blocked(false) {}
  ull value;
  bool blocked;
  Color::Modifier tileColor(ull);
};

class RandInt {
public:
  using clock = std::chrono::system_clock;
  RandInt() : dist{0, std::numeric_limits<int>::max()} {
    seed(clock::now().time_since_epoch().count());
  }
  RandInt(const int low, const int high) : dist{low, high} {
    seed(clock::now().time_since_epoch().count());
  }
  int operator()() { return dist(re); }
  void seed(const unsigned int s) { re.seed(s); }

private:
  std::minstd_rand re;
  std::uniform_int_distribution<> dist;
};

class GameBoard {
  std::vector<std::vector<Tile>> board;

public:
  Tile getTile(int x, int y) const { return board[x][y]; }
  void setTile(int x, int y, Tile tile) { board[x][y] = tile; }
  ull getTileValue(int x, int y) const { return board[x][y].value; }
  void setTileValue(int x, int y, ull value) { board[x][y].value = value; }
  bool getTileBlocked(int x, int y) const { return board[x][y].blocked; }
  void setTileBlocked(int x, int y, bool blocked) {
    board[x][y].blocked = blocked;
  }
  void pushBackRowData(std::vector<Tile> rowData) { board.push_back(rowData); }
};

class Game {

private:
  bool moved;
  bool win;
  bool boardFull;
  bool rexit;
  ull score;
  ull bestScore;
  ull largestTile;
  long long moveCount;
  double duration;
  ull gameBoardPlaySize;
  GameBoard gamePlayBoard;
  RandInt randInt;
  bool stateSaved;
  bool noSave;

  enum ContinueStatus { STATUS_END_GAME = 0, STATUS_CONTINUE = 1 };
  enum KeyInputErrorStatus { STATUS_INPUT_VALID = 0, STATUS_INPUT_ERROR = 1 };
  enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

  void initialiseBoardArray();
  void initialiseContinueBoardArray();
  bool addTile();
  void collectFreeTiles(std::vector<std::tuple<int, int>> &freeTiles);
  void drawBoard();
  void drawScoreBoard(std::ostream &out_stream);
  void input(KeyInputErrorStatus err = STATUS_INPUT_VALID);
  bool canMove();
  bool testAdd(int, int, ull);
  void unblockTiles();
  void decideMove(Directions);
  void move(int, int, int, int);
  void statistics();
  void saveStats();
  void saveScore();
  void saveState();
  void playGame(ContinueStatus);
  void setBoardSize();

public:
  Game()
      : win(false), moved(true), boardFull(false), rexit(false), score(0),
        bestScore(0), moveCount(-2), duration(0.0), largestTile(2),
        stateSaved(false), noSave(false) {}
  void startGame();
  void continueGame();
};

#endif
