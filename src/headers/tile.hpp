#ifndef TILE_H
#define TILE_H

#include "color.hpp"
#include "global.hpp"

class Tile {
public:
  Tile() = default;
  explicit Tile(ull value, bool blocked) : value{value}, blocked{blocked} {}
  ull value{0};
  bool blocked{false};
  Color::Modifier tileColor(ull);
};

#endif
