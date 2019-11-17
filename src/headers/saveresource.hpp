#ifndef SAVERESOURCE_H
#define SAVERESOURCE_H

#include <string>
#include <tuple>

namespace Game {
struct GameBoard;

namespace Saver {
void saveGamePlayState(GameBoard gb);
} // namespace Saver
} // namespace Game

#endif
