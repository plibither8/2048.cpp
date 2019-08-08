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

} // namespace

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
} // namespace Saver
} // namespace Game
