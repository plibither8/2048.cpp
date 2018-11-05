#ifndef GAMEBOARD_H
#define GAMEBOARD_H

#include <algorithm>
#include <chrono>
#include <functional>
#include <global.hpp>
#include <limits>
#include <point2d.hpp>
#include <random>
#include <sstream>
#include <tile.hpp>

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

class GameBoard {
  std::vector<Tile> board;
  ull playsize{0};
  int point2D_to_1D_index(point2D_t pt) const {
    int x, y;
    std::tie(x, y) = pt.get();
    return x + playsize * y;
  }
  bool testAdd(point2D_t pt, ull value) const {
    int x, y;
    std::tie(x, y) = pt.get();
    if (y < 0 || y > getPlaySize() - 1 || x < 0 || x > getPlaySize() - 1) {
      return false;
    }
    return getTileValue(pt) == value;
  }
  std::vector<point2D_t> collectFreeTiles() const {
    std::vector<point2D_t> freeTiles;
    auto index_counter{0};
    const auto gatherFreePoint = [this, &freeTiles,
                                  &index_counter](const Tile t) {
      const auto current_point = point2D_t{index_counter % getPlaySize(),
                                           index_counter / getPlaySize()};
      if (!t.value) {
        freeTiles.push_back(current_point);
      }
      index_counter++;
    };
    std::for_each(std::begin(board), std::end(board), gatherFreePoint);
    return freeTiles;
  }

public:
  GameBoard() = default;
  explicit GameBoard(ull playsize)
      : playsize{playsize}, board{std::vector<Tile>(playsize * playsize)} {}

  Tile getTile(point2D_t pt) const { return board[point2D_to_1D_index(pt)]; }
  void setTile(point2D_t pt, Tile tile) {
    board[point2D_to_1D_index(pt)] = tile;
  }
  ull getTileValue(point2D_t pt) const {
    return board[point2D_to_1D_index(pt)].value;
  }
  void setTileValue(point2D_t pt, ull value) {
    board[point2D_to_1D_index(pt)].value = value;
  }
  bool getTileBlocked(point2D_t pt) const {
    return board[point2D_to_1D_index(pt)].blocked;
  }
  void setTileBlocked(point2D_t pt, bool blocked) {
    board[point2D_to_1D_index(pt)].blocked = blocked;
  }
  void clearGameBoard() { board = std::vector<Tile>(playsize * playsize); }
  int getPlaySize() const { return playsize; }
  void setPlaySize(ull newSize) { playsize = newSize; }
  void unblockTiles() {
    std::transform(std::begin(board), std::end(board), std::begin(board),
                   [](const Tile t) {
                     return Tile{t.value, false};
                   });
  }
  bool canMove() {
    auto index_counter{0};

    const auto predicate = [this, &index_counter](const Tile t) {
      const auto current_point = point2D_t{index_counter % getPlaySize(),
                                           index_counter / getPlaySize()};
      index_counter++;
      const auto list_of_offsets = {point2D_t{1, 0}, point2D_t{0, 1}};
      const auto current_point_value = getTileValue(current_point);

      const auto check_point_offset_in_range = [=](const point2D_t offset) {
        return testAdd(current_point + offset,
                       current_point_value) // Positive adjacent check
               || testAdd(current_point - offset,
                          current_point_value); // Negative adjacent Check
      };

      if (!current_point_value ||
          std::any_of(std::begin(list_of_offsets), std::end(list_of_offsets),
                      check_point_offset_in_range)) {
        return true;
      }
      return false;
    };
    return std::any_of(std::begin(board), std::end(board), predicate);
  }

  bool addTile() {
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

  std::string drawSelf() const {
    std::stringstream str_os;
    using stringlines_t =
        std::tuple<std::stringstream, std::stringstream, std::stringstream>;
    stringlines_t vertibar;

    enum { TOP_BAR, XN_BAR, BASE_BAR };

    using bar_pattern_t = std::tuple<decltype(std::ref(str_os)), std::string,
                                     std::string, std::string>;

    const auto bar_pattern_list = {
        std::make_tuple(std::ref(std::get<TOP_BAR>(vertibar)), "┌", "┬", "┐"),
        std::make_tuple(std::ref(std::get<XN_BAR>(vertibar)), "├", "┼", "┤"),
        std::make_tuple(std::ref(std::get<BASE_BAR>(vertibar)), "└", "┴", "┘")};

    // generate types of horizontal bars...
    // done via LUT...
    const auto generate_x_bar_pattern = [this](bar_pattern_t t) {
      enum { PATTERN_BUFFER, PATTERN_HEAD, PATTERN_MID, PATTERN_TAIL };
      std::get<PATTERN_BUFFER>(t).get() << "  ";
      std::get<PATTERN_BUFFER>(t).get() << std::get<PATTERN_HEAD>(t);
      for (int i = 0; i < getPlaySize(); i++) {
        const auto is_not_last_column = (i < getPlaySize() - 1);
        std::get<PATTERN_BUFFER>(t).get() << "──────";
        std::get<PATTERN_BUFFER>(t).get()
            << (is_not_last_column ? std::get<PATTERN_MID>(t) :
                                     std::get<PATTERN_TAIL>(t));
      }
      std::get<PATTERN_BUFFER>(t).get() << "\n";
    };
    std::for_each(std::begin(bar_pattern_list), std::end(bar_pattern_list),
                  generate_x_bar_pattern);

    for (int y = 0; y < getPlaySize(); y++) {
      const auto is_first_row = (y == 0);
      str_os << (is_first_row ? std::get<TOP_BAR>(vertibar) :
                                std::get<XN_BAR>(vertibar))
                    .str();
      str_os << " ";

      for (int x = 0; x < getPlaySize(); x++) {
        str_os << " │ ";
        Tile currentTile = getTile(point2D_t{x, y});
        if (!currentTile.value) {
          str_os << "    ";
        } else {
          str_os << currentTile.tileColor(currentTile.value) << bold_on
                 << std::setw(4) << currentTile.value << bold_off << def;
        }
      }

      str_os << " │ ";
      str_os << "\n";
    }

    str_os << std::get<BASE_BAR>(vertibar).str();
    str_os << "\n\n\n";
    return str_os.str();
  }

  friend std::ostream &operator<<(std::ostream &os, const GameBoard& gb) {
    return os << gb.drawSelf();
  }
};

#endif
