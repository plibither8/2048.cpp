#include "loadresource.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

namespace Game {
namespace Loader {
namespace {

int GetLines(std::string filename) {
  std::ifstream stateFile(filename);
  using iter = std::istreambuf_iterator<char>;
  const auto noOfLines = std::count(iter{stateFile}, iter{}, '\n');
  return noOfLines;
}

std::vector<std::string> get_file_tile_data(std::istream &buf) {
  std::vector<std::string> tempbuffer;
  enum { MAX_WIDTH = 10, MAX_HEIGHT = 10 };
  auto i{0};
  for (std::string tempLine; std::getline(buf, tempLine) && i < MAX_WIDTH;
       i++) {
    std::istringstream temp_filestream(tempLine);
    auto j{0};
    for (std::string a_word;
         std::getline(temp_filestream, a_word, ',') && j < MAX_HEIGHT; j++) {
      tempbuffer.push_back(a_word);
    }
  }
  return tempbuffer;
}

std::vector<tile_t>
process_file_tile_string_data(std::vector<std::string> buf) {
  std::vector<tile_t> result_buf;
  auto tile_processed_counter{0};
  const auto prime_tile_data =
      [&tile_processed_counter](const std::string tile_data) {
        enum FieldIndex { IDX_TILE_VALUE, IDX_TILE_BLOCKED, MAX_NO_TILE_IDXS };
        std::array<int, MAX_NO_TILE_IDXS> tile_internal{};
        std::istringstream blocks(tile_data);
        auto idx_id{0};
        for (std::string temptiledata; std::getline(
                 blocks, temptiledata, ':') /*&& idx_id < MAX_NO_TILE_IDXS*/;
             idx_id++) {
          switch (idx_id) {
          case IDX_TILE_VALUE:
            std::get<IDX_TILE_VALUE>(tile_internal) = std::stoi(temptiledata);
            break;
          case IDX_TILE_BLOCKED:
            std::get<IDX_TILE_BLOCKED>(tile_internal) = std::stoi(temptiledata);
            break;
          default:
            std::cout << "ERROR: [tile_processed_counter: "
                      << tile_processed_counter
                      << "]: Read past MAX_NO_TILE_IDXS! (idx no:"
                      << MAX_NO_TILE_IDXS << ")\n";
          }
        }
        tile_processed_counter++;
        const unsigned long long tile_value =
            std::get<IDX_TILE_VALUE>(tile_internal);
        const bool tile_blocked = std::get<IDX_TILE_BLOCKED>(tile_internal);
        return tile_t{tile_value, tile_blocked};
      };
  std::transform(std::begin(buf), std::end(buf), std::back_inserter(result_buf),
                 prime_tile_data);
  return result_buf;
}

// TODO: Refactor and combine to one "gameboard" loader function!
std::tuple<bool, std::tuple<unsigned long long, long long>>
get_and_process_game_stats_string_data(std::istream &stats_file) {
  if (stats_file) {
    unsigned long long score;
    long long moveCount;
    for (std::string tempLine; std::getline(stats_file, tempLine);) {
      enum GameStatsFieldIndex {
        IDX_GAME_SCORE_VALUE,
        IDX_GAME_MOVECOUNT,
        MAX_NO_GAME_STATS_IDXS
      };
      std::istringstream line(tempLine);
      auto idx_id{0};
      for (std::string temp; std::getline(line, temp, ':'); idx_id++) {
        switch (idx_id) {
        case IDX_GAME_SCORE_VALUE:
          score = std::stoi(temp);
          break;
        case IDX_GAME_MOVECOUNT:
          moveCount = std::stoi(temp) - 1;
          break;
        default:
          // Error: No fields to process!
          break;
        }
      }
    }
    return std::make_tuple(true, std::make_tuple(score, moveCount));
  }
  return std::make_tuple(false, std::make_tuple(0, 0));
}

} // namespace

load_gameboard_status_t load_GameBoard_data_from_file(std::string filename) {
  std::ifstream stateFile(filename);
  if (stateFile) {
    const ull savedBoardPlaySize = GetLines(filename);
    const auto file_tile_data = get_file_tile_data(stateFile);
    const auto processed_tile_data =
        process_file_tile_string_data(file_tile_data);
    return std::make_tuple(true,
                           GameBoard(savedBoardPlaySize, processed_tile_data));
  }
  return std::make_tuple(false, GameBoard{});
}

// Output: [loadfile_ok_status, gameboard.score, gameboard.moveCount]
std::tuple<bool, std::tuple<unsigned long long, long long>>
load_game_stats_from_file(std::string filename) {
  std::ifstream stats(filename);
  return get_and_process_game_stats_string_data(stats);
}

} // namespace Loader

} // namespace Game
