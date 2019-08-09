#ifndef TILE_H
#define TILE_H

#include "color.hpp"
#include "global.hpp"

struct tile_t {
  ull value{};
  bool blocked{};
};

std::string drawTileString(tile_t currentTile);

#endif
