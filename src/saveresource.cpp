#include "saveresource.hpp"
#include "gameboard.hpp"
#include <fstream>

namespace Game {
namespace Saver {
namespace {

bool generateFilefromPreviousGameStatisticsData(std::ostream &os,
                                                const GameBoard &gb) {
  os << gb.score << ":" << MoveCountOnGameBoard(gb);
  return true;
}

bool generateFilefromPreviousGameStateData(std::ostream &os,
                                           const GameBoard &gb) {
  os << printStateOfGameBoard(gb);
  return true;
}

void saveToFilePreviousGameStateData(std::string filename,
                                     const GameBoard &gb) {
  std::ofstream stateFile(filename, std::ios_base::app);
  generateFilefromPreviousGameStateData(stateFile, gb);
}

void saveToFilePreviousGameStatisticsData(std::string filename,
                                          const GameBoard &gb) {
  std::ofstream stats(filename, std::ios_base::app);
  generateFilefromPreviousGameStatisticsData(stats, gb);
}

} // namespace

void saveGamePlayState(GameBoard gb) {
  // Currently two datafiles for now.
  // Will be merged into one datafile in a future PR.
  constexpr auto filename_game_data_state = "../data/previousGame";
  constexpr auto filename_game_data_statistics = "../data/previousGameStats";
  std::remove(filename_game_data_state);
  std::remove(filename_game_data_statistics);

  saveToFilePreviousGameStateData(filename_game_data_state, gb);
  saveToFilePreviousGameStatisticsData(filename_game_data_statistics, gb);
}

} // namespace Saver
} // namespace Game
