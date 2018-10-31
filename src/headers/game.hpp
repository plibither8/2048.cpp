#ifndef GAME_H
#define GAME_H

#include "color.hpp"
#include "global.hpp"
#include "point2d.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <algorithm>
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
  Tile() = default;
  explicit Tile(ull value, bool blocked) : value{value}, blocked{blocked} {}
  ull value{0};
  bool blocked{false};
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
  std::vector<Tile> board;
  ull playsize{0};
  int point2D_to_1D_index(point2D_t pt) const {
    int x, y;
    std::tie(x, y) = pt.get();
    return x + playsize * y;
  }
  bool testAdd(point2D_t pt, ull value) const {
    int x, y;
    std::tie(x, y) = pt.get();
    if (y < 0 || y > getPlaySize() - 1 || x < 0 || x > getPlaySize() - 1) {
      return false;
    }
    return getTileValue(pt) == value;
  }

public:
  GameBoard() = default;
  explicit GameBoard(ull playsize)
      : playsize{playsize}, board{std::vector<Tile>(playsize * playsize)} {}

  Tile getTile(point2D_t pt) const { return board[point2D_to_1D_index(pt)]; }
  void setTile(point2D_t pt, Tile tile) {
    board[point2D_to_1D_index(pt)] = tile;
  }
  ull getTileValue(point2D_t pt) const {
    return board[point2D_to_1D_index(pt)].value;
  }
  void setTileValue(point2D_t pt, ull value) {
    board[point2D_to_1D_index(pt)].value = value;
  }
  bool getTileBlocked(point2D_t pt) const {
    return board[point2D_to_1D_index(pt)].blocked;
  }
  void setTileBlocked(point2D_t pt, bool blocked) {
    board[point2D_to_1D_index(pt)].blocked = blocked;
  }
  void clearGameBoard() { board = std::vector<Tile>(playsize * playsize); }
  int getPlaySize() const { return playsize; }
  void setPlaySize(ull newSize) { playsize = newSize; }
  void unblockTiles() {
    std::transform(std::begin(board), std::end(board), std::begin(board),
                   [](const Tile t) {
                     return Tile{t.value, false};
                   });
  }
  std::vector<point2D_t> collectFreeTiles() const {
    std::vector<point2D_t> freeTiles;
    auto index_counter{0};
    const auto gatherFreePoint = [this, &freeTiles,
                                  &index_counter](const Tile t) {
      const auto current_point = point2D_t{index_counter % getPlaySize(),
                                           index_counter / getPlaySize()};
      if (!t.value) {
        freeTiles.push_back(current_point);
      }
      index_counter++;
    };
    std::for_each(std::begin(board), std::end(board), gatherFreePoint);
    return freeTiles;
  }
  bool canMove() {
    auto index_counter{0};

    const auto predicate = [this, &index_counter](const Tile t) {
      const auto current_point = point2D_t{index_counter % getPlaySize(),
                                           index_counter / getPlaySize()};
      index_counter++;
      const auto list_of_offsets = {point2D_t{1, 0}, point2D_t{0, 1}};
      const auto current_point_value = getTileValue(current_point);

      const auto check_point_offset_in_range = [=](const point2D_t offset) {
        return testAdd(current_point + offset,
                       current_point_value) // Positive adjacent check
               || testAdd(current_point - offset,
                          current_point_value); // Negative adjacent Check
      };

      if (!current_point_value ||
          std::any_of(std::begin(list_of_offsets), std::end(list_of_offsets),
                      check_point_offset_in_range)) {
        return true;
      }
      return false;
    };
    return std::any_of(std::begin(board), std::end(board), predicate);
  }
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
  GameBoard gamePlayBoard;
  RandInt randInt;
  bool stateSaved;
  bool noSave;

  enum ContinueStatus { STATUS_END_GAME = 0, STATUS_CONTINUE = 1 };
  enum KeyInputErrorStatus { STATUS_INPUT_VALID = 0, STATUS_INPUT_ERROR = 1 };
  enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

  void initialiseContinueBoardArray();
  bool addTile();
  void drawBoard();
  void drawScoreBoard(std::ostream &out_stream);
  void input(KeyInputErrorStatus err = STATUS_INPUT_VALID);
  void decideMove(Directions);
  void move(point2D_t pt, point2D_t pt_offset);
  void statistics();
  void saveStats();
  void saveScore();
  void saveState();
  void playGame(ContinueStatus);
  ull setBoardSize();

public:
  Game()
      : win(false), moved(true), boardFull(false), rexit(false), score(0),
        bestScore(0), moveCount(-2), duration(0.0), largestTile(2),
        stateSaved(false), noSave(false) {}
  void startGame();
  void continueGame();
};

#endif
