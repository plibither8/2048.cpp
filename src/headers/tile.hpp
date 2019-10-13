#ifndef TILE_H
#define TILE_H

#include "global.hpp"

namespace Game {
struct tile_t {
  ull value{};
  bool blocked{};
};
} // namespace Game

#endif
