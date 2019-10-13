#include "gameboard.hpp"
#include "gameboard-graphics.hpp"
#include "point2d.hpp"
#include <algorithm>
#include <array>
#include <chrono>
#include <random>
#include <sstream>
#include <utility>

namespace Game {

namespace {

class RandInt {
public:
  using clock = std::chrono::system_clock;
  RandInt() : dist{0, std::numeric_limits<int>::max()} {
    seed(clock::now().time_since_epoch().count());
  }
  RandInt(const int low, const int high) : dist{low, high} {
    seed(clock::now().time_since_epoch().count());
  }
  int operator()() { return dist(re); }
  void seed(const unsigned int s) { re.seed(s); }

private:
  std::minstd_rand re;
  std::uniform_int_distribution<> dist;
};

using gameboard_data_array_t = GameBoard::gameboard_data_array_t;
enum gameboard_data_array_fields { IDX_PLAYSIZE, IDX_BOARD, MAX_NO_INDEXES };

struct gameboard_data_point_t {
  static int point2D_to_1D_index(gameboard_data_array_t gbda, point2D_t pt) {
    int x, y;
    std::tie(x, y) = pt.get();
    return x + getPlaySizeOfGameboardDataArray(gbda) * y;
  }

  tile_t operator()(gameboard_data_array_t gbda, point2D_t pt) const {
    return std::get<IDX_BOARD>(gbda)[point2D_to_1D_index(gbda, pt)];
  }
  tile_t &operator()(gameboard_data_array_t &gbda, point2D_t pt) {
    return std::get<IDX_BOARD>(gbda)[point2D_to_1D_index(gbda, pt)];
  }
};

void setTileOnGameboardDataArray(gameboard_data_array_t &gbda, point2D_t pt,
                                 tile_t tile) {
  gameboard_data_point_t{}(gbda, pt) = tile;
}

ull getTileValueOnGameboardDataArray(gameboard_data_array_t gbda,
                                     point2D_t pt) {
  return gameboard_data_point_t{}(gbda, pt).value;
}

void setTileValueOnGameboardDataArray(gameboard_data_array_t &gbda,
                                      point2D_t pt, ull value) {
  gameboard_data_point_t{}(gbda, pt).value = value;
}

bool getTileBlockedOnGameboardDataArray(gameboard_data_array_t gbda,
                                        point2D_t pt) {
  return gameboard_data_point_t{}(gbda, pt).blocked;
}

std::string printStateOfGameBoardDataArray(gameboard_data_array_t gbda) {
  const int playsize = getPlaySizeOfGameboardDataArray(gbda);
  std::ostringstream os;
  for (auto y = 0; y < playsize; y++) {
    for (auto x = 0; x < playsize; x++) {
      const auto current_point = point2D_t{x, y};
      os << getTileValueOnGameboardDataArray(gbda, current_point) << ":"
         << getTileBlockedOnGameboardDataArray(gbda, current_point) << ",";
    }
    os << "\n";
  }
  return os.str();
}

bool is_point_in_board_play_area(point2D_t pt, int playsize) {
  int x, y;
  std::tie(x, y) = pt.get();
  return !(y < 0 || y > playsize - 1 || x < 0 || x > playsize - 1);
}

using delta_t = std::pair<point2D_t, point2D_t>;
// NOTE: delta_t.first = focal point, delta_t.second = offset distance

bool check_recursive_offset_in_game_bounds(delta_t dt_point, int playsize) {
  int x, y, x2, y2;
  std::tie(x, y) = dt_point.first.get();
  std::tie(x2, y2) = dt_point.second.get();
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

gameboard_data_array_t
unblockTilesOnGameboardDataArray(gameboard_data_array_t gbda) {
  using tile_data_array_t = GameBoard::tile_data_array_t;
  auto new_board_data_array =
      tile_data_array_t(std::get<IDX_BOARD>(gbda).size());
  std::transform(std::begin(std::get<IDX_BOARD>(gbda)),
                 std::end(std::get<IDX_BOARD>(gbda)),
                 std::begin(new_board_data_array), [](const tile_t t) {
                   return tile_t{t.value, false};
                 });
  return gameboard_data_array_t{std::get<IDX_PLAYSIZE>(gbda),
                                new_board_data_array};
}

bool canMoveOnGameboardDataArray(gameboard_data_array_t gbda) {
  auto index_counter{0};

  const auto can_move_to_offset = [=, &index_counter](const tile_t t) {
    const int playsize = getPlaySizeOfGameboardDataArray(gbda);
    const auto current_point =
        point2D_t{index_counter % playsize, index_counter / playsize};
    index_counter++;
    const auto list_of_offsets = {point2D_t{1, 0}, point2D_t{0, 1}};
    const auto current_point_value = t.value;

    const auto offset_in_range_with_same_value = [=](const point2D_t offset) {
      const auto offset_check = {
          current_point + offset, // Positive adjacent check
          current_point - offset}; // Negative adjacent Check
      for (const auto current_offset : offset_check) {
        if (is_point_in_board_play_area(current_offset, playsize)) {
          return getTileValueOnGameboardDataArray(gbda, current_offset) ==
                 current_point_value;
        }
      }
      return false;
    };

    return ((current_point_value == 0u) ||
            std::any_of(std::begin(list_of_offsets), std::end(list_of_offsets),
                        offset_in_range_with_same_value));
  };
  return std::any_of(std::begin(std::get<IDX_BOARD>(gbda)),
                     std::end(std::get<IDX_BOARD>(gbda)), can_move_to_offset);
}

std::vector<size_t>
collectFreeTilesOnGameboardDataArray(gameboard_data_array_t gbda) {
  std::vector<size_t> freeTiles;
  auto index_counter{0};
  for (const auto t : std::get<IDX_BOARD>(gbda)) {
    if (!t.value) {
      freeTiles.push_back(index_counter);
    }
    index_counter++;
  }
  return freeTiles;
}

bool addTileOnGameboardDataArray(gameboard_data_array_t &gbda) {
  constexpr auto CHANCE_OF_VALUE_FOUR_OVER_TWO = 89; // Percentage
  const auto index_list_of_free_tiles =
      collectFreeTilesOnGameboardDataArray(gbda);

  if (!index_list_of_free_tiles.size()) {
    return true;
  }

  const int playsize = getPlaySizeOfGameboardDataArray(gbda);
  const int rand_selected_index = index_list_of_free_tiles.at(
      RandInt{}() % index_list_of_free_tiles.size());
  const auto rand_index_as_point_t =
      point2D_t{rand_selected_index % playsize, rand_selected_index / playsize};
  const auto value_four_or_two =
      RandInt{}() % 100 > CHANCE_OF_VALUE_FOUR_OVER_TWO ? 4 : 2;
  setTileValueOnGameboardDataArray(gbda, rand_index_as_point_t,
                                   value_four_or_two);

  return false;
}

bool collaspeTilesOnGameboardDataArray(gameboard_data_array_t &gbda,
                                       delta_t dt_point) {
  tile_t currentTile = getTileOnGameboardDataArray(gbda, dt_point.first);
  tile_t targetTile =
      getTileOnGameboardDataArray(gbda, dt_point.first + dt_point.second);

  currentTile.value = 0;
  targetTile.value *= 2;
  targetTile.blocked = true;

  setTileOnGameboardDataArray(gbda, dt_point.first, currentTile);
  setTileOnGameboardDataArray(gbda, dt_point.first + dt_point.second,
                              targetTile);
  return true;
}

bool shiftTilesOnGameboardDataArray(gameboard_data_array_t &gbda,
                                    delta_t dt_point) {
  tile_t currentTile = getTileOnGameboardDataArray(gbda, dt_point.first);
  tile_t targetTile =
      getTileOnGameboardDataArray(gbda, dt_point.first + dt_point.second);

  targetTile.value = currentTile.value;
  currentTile.value = 0;

  setTileOnGameboardDataArray(gbda, dt_point.first, currentTile);
  setTileOnGameboardDataArray(gbda, dt_point.first + dt_point.second,
                              targetTile);
  return true;
}

enum class COLLASPE_OR_SHIFT_T {
  ACTION_NONE,
  ACTION_COLLASPE,
  ACTION_SHIFT,
  MAX_NUM_OF_ACTIONS
};

using bool_collaspe_shift_t = std::tuple<bool, COLLASPE_OR_SHIFT_T>;

bool_collaspe_shift_t
collasped_or_shifted_tilesOnGameboardDataArray(gameboard_data_array_t gbda,
                                               delta_t dt_point) {
  const auto currentTile = getTileOnGameboardDataArray(gbda, dt_point.first);
  const auto targetTile =
      getTileOnGameboardDataArray(gbda, dt_point.first + dt_point.second);
  const auto does_value_exist_in_target_point = targetTile.value;
  const auto is_value_same_as_target_value =
      (currentTile.value == targetTile.value);
  const auto no_tiles_are_blocked =
      (!currentTile.blocked && !targetTile.blocked);
  const auto is_there_a_current_value_but_no_target_value =
      (currentTile.value && !targetTile.value);
  const auto do_collapse =
      (does_value_exist_in_target_point && is_value_same_as_target_value &&
       no_tiles_are_blocked);
  const auto do_shift = is_there_a_current_value_but_no_target_value;
  const auto action_taken = (do_collapse || do_shift);

  if (do_collapse) {
    return std::make_tuple(action_taken, COLLASPE_OR_SHIFT_T::ACTION_COLLASPE);
  } else if (do_shift) {
    return std::make_tuple(action_taken, COLLASPE_OR_SHIFT_T::ACTION_SHIFT);
  }
  return std::make_tuple(action_taken, COLLASPE_OR_SHIFT_T::ACTION_NONE);
}

bool updateGameBoardStats(GameBoard &gb, ull target_tile_value) {
  gb.score += target_tile_value;

  //  Discover the largest tile value on the gameboard...
  gb.largestTile = std::max(gb.largestTile, target_tile_value);

  //  Discover the winning tile value on the gameboard...
  if (!hasWonOnGameboard(gb)) {
    constexpr auto GAME_TILE_WINNING_SCORE = 2048;
    if (target_tile_value == GAME_TILE_WINNING_SCORE) {
      gb.win = true;
    }
  }
  return true;
}

void moveOnGameboard(GameBoard &gb, delta_t dt_point) {
  auto did_gameboard_collaspe_or_shift_anything = bool{};
  auto action_was_taken = COLLASPE_OR_SHIFT_T::ACTION_NONE;
  std::tie(did_gameboard_collaspe_or_shift_anything, action_was_taken) =
      collasped_or_shifted_tilesOnGameboardDataArray(gb.gbda, dt_point);
  if (did_gameboard_collaspe_or_shift_anything) {
    gb.moved = true;
    if (action_was_taken == COLLASPE_OR_SHIFT_T::ACTION_COLLASPE) {
      collaspeTilesOnGameboardDataArray(gb.gbda, dt_point);
      const auto targetTile = getTileOnGameboardDataArray(
          gb.gbda, dt_point.first + dt_point.second);
      updateGameBoardStats(gb, targetTile.value);
    }
    if (action_was_taken == COLLASPE_OR_SHIFT_T::ACTION_SHIFT) {
      shiftTilesOnGameboardDataArray(gb.gbda, dt_point);
    }
  }
  if (check_recursive_offset_in_game_bounds(
          dt_point, getPlaySizeOfGameboardDataArray(gb.gbda))) {
    moveOnGameboard(
        gb, std::make_pair(dt_point.first + dt_point.second, dt_point.second));
  }
}

void doTumbleTilesUpOnGameboard(GameBoard &gb) {
  const int playsize = getPlaySizeOfGameboardDataArray(gb.gbda);
  for (auto x = 0; x < playsize; x++) {
    auto y = 1;
    while (y < playsize) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboardDataArray(gb.gbda, current_point)) {
        moveOnGameboard(gb, std::make_pair(current_point, point2D_t{0, -1}));
      }
      y++;
    }
  }
}

void doTumbleTilesDownOnGameboard(GameBoard &gb) {
  const int playsize = getPlaySizeOfGameboardDataArray(gb.gbda);
  for (auto x = 0; x < playsize; x++) {
    auto y = playsize - 2;
    while (y >= 0) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboardDataArray(gb.gbda, current_point)) {
        moveOnGameboard(gb, std::make_pair(current_point, point2D_t{0, 1}));
      }
      y--;
    }
  }
}

void doTumbleTilesLeftOnGameboard(GameBoard &gb) {
  const int playsize = getPlaySizeOfGameboardDataArray(gb.gbda);
  for (auto y = 0; y < playsize; y++) {
    auto x = 1;
    while (x < playsize) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboardDataArray(gb.gbda, current_point)) {
        moveOnGameboard(gb, std::make_pair(current_point, point2D_t{-1, 0}));
      }
      x++;
    }
  }
}

void doTumbleTilesRightOnGameboard(GameBoard &gb) {
  const int playsize = getPlaySizeOfGameboardDataArray(gb.gbda);
  for (auto y = 0; y < playsize; y++) {
    auto x = playsize - 2;
    while (x >= 0) {
      const auto current_point = point2D_t{x, y};
      if (getTileValueOnGameboardDataArray(gb.gbda, current_point)) {
        moveOnGameboard(gb, std::make_pair(current_point, point2D_t{1, 0}));
      }
      x--;
    }
  }
}

} // namespace

GameBoard::GameBoard(ull playsize)
    : GameBoard{playsize, tile_data_array_t(playsize * playsize)} {
}
GameBoard::GameBoard(ull playsize, tile_data_array_t prempt_board)
    : gbda{playsize, prempt_board} {
}

size_t getPlaySizeOfGameboardDataArray(gameboard_data_array_t gbda) {
  return std::get<IDX_PLAYSIZE>(gbda);
}

tile_t getTileOnGameboardDataArray(gameboard_data_array_t gbda, point2D_t pt) {
  return gameboard_data_point_t{}(gbda, pt);
}

bool hasWonOnGameboard(GameBoard gb) {
  return gb.win;
}

long long MoveCountOnGameBoard(GameBoard gb) {
  return gb.moveCount;
}

void unblockTilesOnGameboard(GameBoard &gb) {
  gb.gbda = unblockTilesOnGameboardDataArray(gb.gbda);
}

bool canMoveOnGameboard(GameBoard &gb) {
  return canMoveOnGameboardDataArray(gb.gbda);
}

void registerMoveByOneOnGameboard(GameBoard &gb) {
  gb.moveCount++;
  gb.moved = false;
}

bool addTileOnGameboard(GameBoard &gb) {
  return addTileOnGameboardDataArray(gb.gbda);
}

void tumbleTilesUpOnGameboard(GameBoard &gb) {
  doTumbleTilesUpOnGameboard(gb);
}

void tumbleTilesDownOnGameboard(GameBoard &gb) {
  doTumbleTilesDownOnGameboard(gb);
}

void tumbleTilesLeftOnGameboard(GameBoard &gb) {
  doTumbleTilesLeftOnGameboard(gb);
}

void tumbleTilesRightOnGameboard(GameBoard &gb) {
  doTumbleTilesRightOnGameboard(gb);
}

std::string printStateOfGameBoard(GameBoard gb) {
  return printStateOfGameBoardDataArray(gb.gbda);
}

} // namespace Game
