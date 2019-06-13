#include "gameboard.hpp"
#include <algorithm>
#include <array>
#include <iomanip>
#include <sstream>

namespace {

// Pre-declare function signature for organisational reasons.
Tile getTileOnGameboard(GameBoard gb, point2D_t pt);

std::string drawTileString(Tile currentTile) {
  std::ostringstream tile_richtext;
  if (!currentTile.value) {
    tile_richtext << "    ";
  } else {
    tile_richtext << currentTile.tileColor(currentTile.value) << bold_on
                  << std::setw(4) << currentTile.value << bold_off << def;
  }
  return tile_richtext.str();
}

template<int num_of_bars>
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
    for (int i = 0; i < playsize; i++) {
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

std::string drawSelf(GameBoard gb) {
  enum { TOP_BAR, XN_BAR, BASE_BAR, MAX_TYPES_OF_BARS };
  const auto vertibar =
      make_patterned_bars<MAX_TYPES_OF_BARS>(getPlaySizeOfGameboard(gb));
  std::ostringstream str_os;
  for (int y = 0; y < getPlaySizeOfGameboard(gb); y++) {
    const auto is_first_row = (y == 0);
    str_os << (is_first_row ? std::get<TOP_BAR>(vertibar) :
                              std::get<XN_BAR>(vertibar));
    for (int x = 0; x < getPlaySizeOfGameboard(gb); x++) {
      const auto is_first_col = (x == 0);
      const auto sp = (is_first_col ? "  " : " ");
      str_os << sp;
      str_os << "│ ";
      str_os << drawTileString(getTileOnGameboard(gb, point2D_t{x, y}));
    }
    str_os << " │";
    str_os << "\n";
  }
  str_os << std::get<BASE_BAR>(vertibar);
  str_os << "\n";
  return str_os.str();
}

bool is_point_in_board_play_area(point2D_t pt, int playsize) {
  int x, y;
  std::tie(x, y) = pt.get();
  return !(y < 0 || y > playsize - 1 || x < 0 || x > playsize - 1);
}

bool check_recursive_offset_in_game_bounds(point2D_t pt, point2D_t pt_offset,
                                           int playsize) {
  int x, y, x2, y2;
  std::tie(x, y) = pt.get();
  std::tie(x2, y2) = pt_offset.get();
  const auto positive_direction = (y2 + x2 == 1);
  const auto negative_direction = (y2 + x2 == -1);
  const auto is_positive_y_direction_flagged = (y2 == 1);
  const auto is_negative_y_direction_flagged = (y2 == -1);
  const auto is_inside_outer_bounds =
      (positive_direction &&
       (is_positive_y_direction_flagged ? y : x) < playsize - 2);
  const auto is_inside_inner_bounds =
      (negative_direction && (is_negative_y_direction_flagged ? y : x) > 1);
  return (is_inside_outer_bounds || is_inside_inner_bounds);
}

int point2D_to_1D_index(GameBoard gb, point2D_t pt) {
  int x, y;
  std::tie(x, y) = pt.get();
  return x + getPlaySizeOfGameboard(gb) * y;
}

std::vector<point2D_t> collectFreeTilesOnGameboard(GameBoard gb) {
  std::vector<point2D_t> freeTiles;
  auto index_counter{0};
  const auto gatherFreePoint = [gb, &freeTiles, &index_counter](const Tile t) {
    const auto current_point =
        point2D_t{index_counter % getPlaySizeOfGameboard(gb),
                  index_counter / getPlaySizeOfGameboard(gb)};
    if (!t.value) {
      freeTiles.push_back(current_point);
    }
    index_counter++;
  };
  std::for_each(std::begin(gb.board), std::end(gb.board), gatherFreePoint);
  return freeTiles;
}

Tile getTileOnGameboard(GameBoard gb, point2D_t pt) {
  return gb.board[point2D_to_1D_index(gb, pt)];
}

void setTileOnGameboard(GameBoard &gb, point2D_t pt, Tile tile) {
  gb.board[point2D_to_1D_index(gb, pt)] = tile;
}

ull getTileValueOnGameboard(GameBoard gb, point2D_t pt) {
  return gb.board[point2D_to_1D_index(gb, pt)].value;
}

void setTileValueOnGameboard(GameBoard &gb, point2D_t pt, ull value) {
  gb.board[point2D_to_1D_index(gb, pt)].value = value;
}

bool getTileBlockedOnGameboard(GameBoard gb, point2D_t pt) {
  return gb.board[point2D_to_1D_index(gb, pt)].blocked;
}

void discoverLargestTileValueOnGameboard(GameBoard gb, Tile targetTile) {
  gb.largestTile =
      gb.largestTile < targetTile.value ? targetTile.value : gb.largestTile;
}

void discoverWinningTileValueOnGameboard(GameBoard gb, Tile targetTile) {
  if (!hasWonOnGameboard(gb)) {
    constexpr auto GAME_TILE_WINNING_SCORE = 2048;
    if (targetTile.value == GAME_TILE_WINNING_SCORE) {
      gb.win = true;
    }
  }
}

bool collaspeTilesOnGameboard(GameBoard &gb, point2D_t pt,
                              point2D_t pt_offset) {
  Tile currentTile = getTileOnGameboard(gb, pt);
  Tile targetTile = getTileOnGameboard(gb, pt + pt_offset);

  currentTile.value = 0;
  targetTile.value *= 2;
  gb.score += targetTile.value;
  targetTile.blocked = true;

  discoverLargestTileValueOnGameboard(gb, targetTile);
  discoverWinningTileValueOnGameboard(gb, targetTile);

  setTileOnGameboard(gb, pt, currentTile);
  setTileOnGameboard(gb, pt + pt_offset, targetTile);
  return true;
}

bool shiftTilesOnGameboard(GameBoard &gb, point2D_t pt, point2D_t pt_offset) {
  Tile currentTile = getTileOnGameboard(gb, pt);
  Tile targetTile = getTileOnGameboard(gb, pt + pt_offset);

  targetTile.value = currentTile.value;
  currentTile.value = 0;

  setTileOnGameboard(gb, pt, currentTile);
  setTileOnGameboard(gb, pt + pt_offset, targetTile);
  return true;
}

bool collasped_or_shifted_tilesOnGameboard(GameBoard &gb, point2D_t pt,
                                           point2D_t pt_offset) {
  const auto currentTile = getTileOnGameboard(gb, pt);
  const auto targetTile = getTileOnGameboard(gb, pt + pt_offset);
  const auto does_value_exist_in_target_point = targetTile.value;
  const auto is_value_same_as_target_value =
      (currentTile.value == targetTile.value);
  const auto no_tiles_are_blocked =
      (!currentTile.blocked && !targetTile.blocked);
  const auto is_there_a_current_value_but_no_target_value =
      (currentTile.value && !targetTile.value);

  if (does_value_exist_in_target_point && is_value_same_as_target_value &&
      no_tiles_are_blocked) {
    return collaspeTilesOnGameboard(gb, pt, pt_offset);
  } else if (is_there_a_current_value_but_no_target_value) {
    return shiftTilesOnGameboard(gb, pt, pt_offset);
  }
  return false;
}

void moveOnGameboard(GameBoard &gb, point2D_t pt, point2D_t pt_offset) {
  if (collasped_or_shifted_tilesOnGameboard(gb, pt, pt_offset)) {
    gb.moved = true;
  }
  if (check_recursive_offset_in_game_bounds(pt, pt_offset,
                                            getPlaySizeOfGameboard(gb))) {
    moveOnGameboard(gb, pt + pt_offset, pt_offset);
  }
}

} // namespace

int getPlaySizeOfGameboard(GameBoard gb) {
  return gb.playsize;
}

bool hasWonOnGameboard(GameBoard gb) {
  return gb.win;
}

long long MoveCountOnGameBoard(GameBoard gb) {
  return gb.moveCount;
}

void GameBoard::unblockTiles() {
  std::transform(std::begin(board), std::end(board), std::begin(board),
                 [](const Tile t) {
                   return Tile{t.value, false};
                 });
}

bool GameBoard::canMove() {
  auto index_counter{0};

  const auto can_move_to_offset = [this, &index_counter](const Tile t) {
    const auto current_point =
        point2D_t{index_counter % getPlaySizeOfGameboard(*this),
                  index_counter / getPlaySizeOfGameboard(*this)};
    index_counter++;
    const auto list_of_offsets = {point2D_t{1, 0}, point2D_t{0, 1}};
    const auto current_point_value = t.value;

    const auto offset_in_range_with_same_value = [=](const point2D_t offset) {
      const auto offset_check = {
          current_point + offset, // Positive adjacent check
          current_point - offset}; // Negative adjacent Check
      for (const auto current_offset : offset_check) {
        if (is_point_in_board_play_area(current_offset,
                                        getPlaySizeOfGameboard(*this))) {
          return getTileValueOnGameboard(*this, current_offset) ==
                 current_point_value;
        }
      }
      return false;
    };

    return ((current_point_value == 0u) ||
            std::any_of(std::begin(list_of_offsets), std::end(list_of_offsets),
                        offset_in_range_with_same_value));
  };
  return std::any_of(std::begin(board), std::end(board), can_move_to_offset);
}

void GameBoard::registerMoveByOne() {
  moveCount++;
  moved = false;
}

bool GameBoard::addTile() {
  constexpr auto CHANCE_OF_VALUE_FOUR_OVER_TWO = 89; // Percentage
  const auto freeTiles = collectFreeTilesOnGameboard(*this);

  if (!freeTiles.size()) {
    return true;
  }

  const auto randomFreeTile = freeTiles.at(RandInt{}() % freeTiles.size());
  const auto value_four_or_two =
      RandInt{}() % 100 > CHANCE_OF_VALUE_FOUR_OVER_TWO ? 4 : 2;
  setTileValueOnGameboard(*this, randomFreeTile, value_four_or_two);

  return false;
}

void GameBoard::tumbleTilesUp() {
  for (int x = 0; x < getPlaySizeOfGameboard(*this); x++) {
    int y = 1;
    while (y < getPlaySizeOfGameboard(*this)) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboard(*this, current_point)) {
        moveOnGameboard(*this, current_point, point2D_t{0, -1});
      }
      y++;
    }
  }
}

void GameBoard::tumbleTilesDown() {
  for (int x = 0; x < getPlaySizeOfGameboard(*this); x++) {
    int y = getPlaySizeOfGameboard(*this) - 2;
    while (y >= 0) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboard(*this, current_point)) {
        moveOnGameboard(*this, current_point, point2D_t{0, 1});
      }
      y--;
    }
  }
}

void GameBoard::tumbleTilesLeft() {
  for (int y = 0; y < getPlaySizeOfGameboard(*this); y++) {
    int x = 1;
    while (x < getPlaySizeOfGameboard(*this)) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboard(*this, current_point)) {
        moveOnGameboard(*this, current_point, {-1, 0});
      }
      x++;
    }
  }
}

void GameBoard::tumbleTilesRight() {
  for (int y = 0; y < getPlaySizeOfGameboard(*this); y++) {
    int x = getPlaySizeOfGameboard(*this) - 2;
    while (x >= 0) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboard(*this, current_point)) {
        moveOnGameboard(*this, current_point, point2D_t{1, 0});
      }
      x--;
    }
  }
}

std::string printStateOfGameBoard(GameBoard gb) {
  std::stringstream out_stream;
  for (int y = 0; y < getPlaySizeOfGameboard(gb); y++) {
    for (int x = 0; x < getPlaySizeOfGameboard(gb); x++) {
      const auto current_point = point2D_t{x, y};
      out_stream << getTileValueOnGameboard(gb, current_point) << ":"
                 << getTileBlockedOnGameboard(gb, current_point) << ",";
    }
    out_stream << "\n";
  }
  return out_stream.str();
}

std::ostream &operator<<(std::ostream &os, const GameBoard &gb) {
  return os << drawSelf(gb);
}
