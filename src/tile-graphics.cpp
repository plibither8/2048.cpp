#include "tile-graphics.hpp"
#include "color.hpp"
#include "tile.hpp"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <vector>

namespace Game {
namespace {

Color::Modifier tileColor(ull value) {
  std::vector<Color::Modifier> colors{red, yellow, magenta, blue, cyan, yellow,
                                      red, yellow, magenta, blue, green};
  int log = log2(value);
  int index = log < 12 ? log - 1 : 10;

  return colors[index];
}
} // namespace

std::string drawTileString(tile_t currentTile) {
  std::ostringstream tile_richtext;
  if (!currentTile.value) {
    tile_richtext << "    ";
  } else {
    tile_richtext << tileColor(currentTile.value) << bold_on << std::setw(4)
                  << currentTile.value << bold_off << def;
  }
  return tile_richtext.str();
}
} // namespace Game
