#pragma once

#include "gameboard.hpp"
#include <string>
#include <tuple>

namespace Game {
namespace Saver {
void saveToFilePreviousGameStateData(std::string filename, const GameBoard &gb);
void saveToFilePreviousGameStatisticsData(std::string filename,
                                          const GameBoard &gb);
} // namespace Saver
} // namespace Game
