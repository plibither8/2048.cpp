#include "gameboard.hpp"
#include <algorithm>
#include <array>
#include <iomanip>
#include <sstream>

namespace {
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

} // namespace

int GameBoard::point2D_to_1D_index(point2D_t pt) const {
  int x, y;
  std::tie(x, y) = pt.get();
  return x + playsize * y;
}

std::vector<point2D_t> GameBoard::collectFreeTiles() const {
  std::vector<point2D_t> freeTiles;
  auto index_counter{0};
  const auto gatherFreePoint = [this, &freeTiles,
                                &index_counter](const Tile t) {
    const auto current_point =
        point2D_t{index_counter % getPlaySize(), index_counter / getPlaySize()};
    if (!t.value) {
      freeTiles.push_back(current_point);
    }
    index_counter++;
  };
  std::for_each(std::begin(board), std::end(board), gatherFreePoint);
  return freeTiles;
}

Tile GameBoard::getTile(point2D_t pt) const {
  return board[point2D_to_1D_index(pt)];
}

void GameBoard::setTile(point2D_t pt, Tile tile) {
  board[point2D_to_1D_index(pt)] = tile;
}

ull GameBoard::getTileValue(point2D_t pt) const {
  return board[point2D_to_1D_index(pt)].value;
}

void GameBoard::setTileValue(point2D_t pt, ull value) {
  board[point2D_to_1D_index(pt)].value = value;
}

bool GameBoard::getTileBlocked(point2D_t pt) const {
  return board[point2D_to_1D_index(pt)].blocked;
}

void GameBoard::setTileBlocked(point2D_t pt, bool blocked) {
  board[point2D_to_1D_index(pt)].blocked = blocked;
}

int GameBoard::getPlaySize() const {
  return playsize;
}

bool GameBoard::hasWon() const {
  return win;
}

long long GameBoard::MoveCount() const {
  return moveCount;
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
        point2D_t{index_counter % getPlaySize(), index_counter / getPlaySize()};
    index_counter++;
    const auto list_of_offsets = {point2D_t{1, 0}, point2D_t{0, 1}};
    const auto current_point_value = t.value;

    const auto offset_in_range_with_same_value = [=](const point2D_t offset) {
      const auto offset_check = {
          current_point + offset, // Positive adjacent check
          current_point - offset}; // Negative adjacent Check
      for (const auto current_offset : offset_check) {
        if (is_point_in_board_play_area(current_offset, getPlaySize())) {
          return getTileValue(current_offset) == current_point_value;
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
  const auto freeTiles = collectFreeTiles();

  if (!freeTiles.size()) {
    return true;
  }

  const auto randomFreeTile = freeTiles.at(RandInt{}() % freeTiles.size());
  const auto value_four_or_two =
      RandInt{}() % 100 > CHANCE_OF_VALUE_FOUR_OVER_TWO ? 4 : 2;
  setTileValue(randomFreeTile, value_four_or_two);

  return false;
}

std::string GameBoard::drawSelf() const {
  enum { TOP_BAR, XN_BAR, BASE_BAR, MAX_TYPES_OF_BARS };
  const auto vertibar = make_patterned_bars<MAX_TYPES_OF_BARS>(getPlaySize());
  std::ostringstream str_os;
  for (int y = 0; y < getPlaySize(); y++) {
    const auto is_first_row = (y == 0);
    str_os << (is_first_row ? std::get<TOP_BAR>(vertibar) :
                              std::get<XN_BAR>(vertibar));
    for (int x = 0; x < getPlaySize(); x++) {
      const auto is_first_col = (x == 0);
      const auto sp = (is_first_col ? "  " : " ");
      str_os << sp;
      str_os << "│ ";
      str_os << drawTileString(getTile(point2D_t{x, y}));
    }
    str_os << " │";
    str_os << "\n";
  }
  str_os << std::get<BASE_BAR>(vertibar);
  str_os << "\n";
  return str_os.str();
}

bool GameBoard::collaspeTiles(point2D_t pt, point2D_t pt_offset) {
  Tile currentTile = getTile(pt);
  Tile targetTile = getTile(pt + pt_offset);

  currentTile.value = 0;
  targetTile.value *= 2;
  score += targetTile.value;
  targetTile.blocked = true;

  discoverLargestTileValue(targetTile);
  discoverWinningTileValue(targetTile);

  setTile(pt, currentTile);
  setTile(pt + pt_offset, targetTile);
  return true;
}

bool GameBoard::shiftTiles(point2D_t pt, point2D_t pt_offset) {
  Tile currentTile = getTile(pt);
  Tile targetTile = getTile(pt + pt_offset);

  targetTile.value = currentTile.value;
  currentTile.value = 0;

  setTile(pt, currentTile);
  setTile(pt + pt_offset, targetTile);
  return true;
}

bool GameBoard::collasped_or_shifted_tiles(point2D_t pt, point2D_t pt_offset) {
  const auto currentTile = getTile(pt);
  const auto targetTile = getTile(pt + pt_offset);
  const auto does_value_exist_in_target_point = targetTile.value;
  const auto is_value_same_as_target_value =
      (currentTile.value == targetTile.value);
  const auto no_tiles_are_blocked =
      (!currentTile.blocked && !targetTile.blocked);
  const auto is_there_a_current_value_but_no_target_value =
      (currentTile.value && !targetTile.value);

  if (does_value_exist_in_target_point && is_value_same_as_target_value &&
      no_tiles_are_blocked) {
    return collaspeTiles(pt, pt_offset);
  } else if (is_there_a_current_value_but_no_target_value) {
    return shiftTiles(pt, pt_offset);
  }
  return false;
}

void GameBoard::discoverLargestTileValue(Tile targetTile) {
  largestTile = largestTile < targetTile.value ? targetTile.value : largestTile;
}

void GameBoard::discoverWinningTileValue(Tile targetTile) {
  if (!hasWon()) {
    constexpr auto GAME_TILE_WINNING_SCORE = 2048;
    if (targetTile.value == GAME_TILE_WINNING_SCORE) {
      win = true;
    }
  }
}

void GameBoard::move(point2D_t pt, point2D_t pt_offset) {
  if (collasped_or_shifted_tiles(pt, pt_offset)) {
    moved = true;
  }
  if (check_recursive_offset_in_game_bounds(pt, pt_offset, getPlaySize())) {
    move(pt + pt_offset, pt_offset);
  }
}

void GameBoard::tumbleTilesUp() {
  for (int x = 0; x < getPlaySize(); x++) {
    int y = 1;
    while (y < getPlaySize()) {
      const auto current_point = point2D_t{x, y};
      if (getTileValue(current_point)) {
        move(current_point, point2D_t{0, -1});
      }
      y++;
    }
  }
}

void GameBoard::tumbleTilesDown() {
  for (int x = 0; x < getPlaySize(); x++) {
    int y = getPlaySize() - 2;
    while (y >= 0) {
      const auto current_point = point2D_t{x, y};
      if (getTileValue(current_point)) {
        move(current_point, point2D_t{0, 1});
      }
      y--;
    }
  }
}

void GameBoard::tumbleTilesLeft() {
  for (int y = 0; y < getPlaySize(); y++) {
    int x = 1;
    while (x < getPlaySize()) {
      const auto current_point = point2D_t{x, y};
      if (getTileValue(current_point)) {
        move(current_point, {-1, 0});
      }
      x++;
    }
  }
}

void GameBoard::tumbleTilesRight() {
  for (int y = 0; y < getPlaySize(); y++) {
    int x = getPlaySize() - 2;
    while (x >= 0) {
      const auto current_point = point2D_t{x, y};
      if (getTileValue(current_point)) {
        move(current_point, point2D_t{1, 0});
      }
      x--;
    }
  }
}

std::string GameBoard::printState() const {
  std::stringstream out_stream;
  for (int y = 0; y < getPlaySize(); y++) {
    for (int x = 0; x < getPlaySize(); x++) {
      const auto current_point = point2D_t{x, y};
      out_stream << getTileValue(current_point) << ":"
                 << getTileBlocked(current_point) << ",";
    }
    out_stream << "\n";
  }
  return out_stream.str();
}

std::ostream &operator<<(std::ostream &os, const GameBoard &gb) {
  return os << gb.drawSelf();
}
