#ifndef TILE_H
#define TILE_H

#include "color.hpp"
#include "global.hpp"

struct tile_t {
  ull value{};
  bool blocked{};
};

std::ostream &operator<<(std::ostream &os, const tile_t &t);

#endif
