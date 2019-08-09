#ifndef TILE_H
#define TILE_H

#include "color.hpp"
#include "global.hpp"

struct Tile {
  ull value{};
  bool blocked{};
};

std::string drawTileString(Tile currentTile);

#endif
