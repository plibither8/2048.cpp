#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include "point2d.hpp"
#include "tile.hpp"
#include <chrono>
#include <random>
#include <tuple>

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

class GameBoard;
using load_gameboard_status_t = std::tuple<bool, GameBoard>;

class GameBoard {
  ull playsize{0};
  std::vector<Tile> board;
  bool win{};

  Tile getTile(point2D_t pt) const;
  void setTile(point2D_t pt, Tile tile);
  ull getTileValue(point2D_t pt) const;
  bool getTileBlocked(point2D_t pt) const;

  int point2D_to_1D_index(point2D_t pt) const;
  std::vector<point2D_t> collectFreeTiles() const;

  bool collaspeTiles(point2D_t pt, point2D_t pt_offset);

  bool shiftTiles(point2D_t pt, point2D_t pt_offset);

  bool collasped_or_shifted_tiles(point2D_t pt, point2D_t pt_offset);

  void discoverLargestTileValue(Tile targetTile);
  void discoverWinningTileValue(Tile targetTile);

  void move(point2D_t pt, point2D_t pt_offset);

public:
  bool moved{true};
  ull score{};
  ull largestTile{2};
  long long moveCount{-1};

  GameBoard() = default;
  explicit GameBoard(ull playsize)
      : playsize{playsize}, board{std::vector<Tile>(playsize * playsize)} {}
  explicit GameBoard(ull playsize, const std::vector<Tile> &prempt_board)
      : playsize{playsize}, board{prempt_board} {}

  void setTileValue(point2D_t pt, ull value);
  void setTileBlocked(point2D_t pt, bool blocked);
  int getPlaySize() const;
  bool hasWon() const;
  long long MoveCount() const;
  void unblockTiles();
  bool canMove();
  void registerMoveByOne();

  bool addTile();

  void tumbleTilesUp();
  void tumbleTilesDown();
  void tumbleTilesLeft();
  void tumbleTilesRight();

  std::string printState() const;
  std::string drawSelf() const;
  friend std::ostream &operator<<(std::ostream &os, const GameBoard &gb);
};

#endif
