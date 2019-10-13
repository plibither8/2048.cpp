#include "gameboard-graphics.hpp"
#include "gameboard.hpp"
#include "point2d.hpp"
#include "tile-graphics.hpp"
#include "tile.hpp"
#include <algorithm>
#include <array>
#include <sstream>

namespace Game {
namespace Gameboard {
namespace Graphics {
namespace {

template<size_t num_of_bars>
std::array<std::string, num_of_bars> make_patterned_bars(int playsize) {
  auto temp_bars = std::array<std::string, num_of_bars>{};
  using bar_pattern_t = std::tuple<std::string, std::string, std::string>;

  const auto bar_pattern_list = {std::make_tuple("┌", "┬", "┐"),
                                 std::make_tuple("├", "┼", "┤"),
                                 std::make_tuple("└", "┴", "┘")};

  // generate types of horizontal bars...
  const auto generate_x_bar_pattern = [playsize](const bar_pattern_t t) {
    enum { PATTERN_HEAD, PATTERN_MID, PATTERN_TAIL };
    constexpr auto sp = "  ";
    constexpr auto separator = "──────";
    std::ostringstream temp_richtext;
    temp_richtext << sp << std::get<PATTERN_HEAD>(t);
    for (auto i = 0; i < playsize; i++) {
      const auto is_not_last_column = (i < playsize - 1);
      temp_richtext << separator
                    << (is_not_last_column ? std::get<PATTERN_MID>(t) :
                                             std::get<PATTERN_TAIL>(t));
    }
    temp_richtext << "\n";
    return temp_richtext.str();
  };
  std::transform(std::begin(bar_pattern_list), std::end(bar_pattern_list),
                 std::begin(temp_bars), generate_x_bar_pattern);
  return temp_bars;
}

std::string drawSelf(GameBoard::gameboard_data_array_t gbda) {
  enum { TOP_BAR, XN_BAR, BASE_BAR, MAX_TYPES_OF_BARS };
  const int playsize = getPlaySizeOfGameboardDataArray(gbda);
  const auto vertibar = make_patterned_bars<MAX_TYPES_OF_BARS>(playsize);
  std::ostringstream str_os;
  for (auto y = 0; y < playsize; y++) {
    const auto is_first_row = (y == 0);
    str_os << (is_first_row ? std::get<TOP_BAR>(vertibar) :
                              std::get<XN_BAR>(vertibar));
    for (auto x = 0; x < playsize; x++) {
      const auto is_first_col = (x == 0);
      const auto sp = (is_first_col ? "  " : " ");
      const auto tile = getTileOnGameboardDataArray(gbda, point2D_t{x, y});
      str_os << sp;
      str_os << "│ ";
      str_os << drawTileString(tile);
    }
    str_os << " │";
    str_os << "\n";
  }
  str_os << std::get<BASE_BAR>(vertibar);
  str_os << "\n";
  return str_os.str();
}

} // namespace

std::string GameBoardTextOutput(GameBoard gb) {
  return drawSelf(gb.gbda);
}
} // namespace Graphics
} // namespace Gameboard
} // namespace Game
