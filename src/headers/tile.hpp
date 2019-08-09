#ifndef TILE_H
#define TILE_H

#include "color.hpp"
#include "global.hpp"

namespace Game {

struct tile_t {
  ull value{};
  bool blocked{};
};

} // namespace Game

std::ostream &operator<<(std::ostream &os, const Game::tile_t &t);

#endif
