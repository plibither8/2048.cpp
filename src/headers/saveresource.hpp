#ifndef SAVERESOURCE_H
#define SAVERESOURCE_H

#include <string>
#include <tuple>

namespace Game {
struct GameBoard;

namespace Saver {
void saveToFilePreviousGameStateData(std::string filename, const GameBoard &gb);
void saveToFilePreviousGameStatisticsData(std::string filename,
                                          const GameBoard &gb);
} // namespace Saver
} // namespace Game

#endif
