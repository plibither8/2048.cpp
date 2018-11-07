#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <algorithm>
#include <chrono>
#include <functional>
#include <global.hpp>
#include <limits>
#include <point2d.hpp>
#include <random>
#include <sstream>
#include <tile.hpp>

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

  Tile getTile(point2D_t pt) const;
  void setTile(point2D_t pt, Tile tile);

  int point2D_to_1D_index(point2D_t pt) const;
  bool testAdd(point2D_t pt, ull value) const;
  std::vector<point2D_t> collectFreeTiles() const;

  bool collaspeTiles(point2D_t pt, point2D_t pt_offset);

  bool shiftTiles(point2D_t pt, point2D_t pt_offset);

  bool collasped_or_shifted_tiles(point2D_t pt, point2D_t pt_offset);

  bool check_recursive_offset_in_game_bounds(point2D_t pt, point2D_t pt_offset);

  void discoverLargestTileValue(Tile targetTile);
  void discoverWinningTileValue(Tile targetTile);

public:
  bool win{};
  bool moved{true};
  ull score{};
  ull largestTile{2};

  GameBoard() = default;
  explicit GameBoard(ull playsize)
      : playsize{playsize}, board{std::vector<Tile>(playsize * playsize)} {}

  ull getTileValue(point2D_t pt) const;
  void setTileValue(point2D_t pt, ull value);
  bool getTileBlocked(point2D_t pt) const;
  void setTileBlocked(point2D_t pt, bool blocked);
  void clearGameBoard();
  int getPlaySize() const;
  void setPlaySize(ull newSize);
  void unblockTiles();
  bool canMove();

  bool addTile();

  std::string drawSelf() const;

  friend std::ostream &operator<<(std::ostream &os, const GameBoard &gb);

  void move(point2D_t pt, point2D_t pt_offset);

  void tumbleTilesUp();
  void tumbleTilesDown();
  void tumbleTilesLeft();
  void tumbleTilesRight();
};

#endif
