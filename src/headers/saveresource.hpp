#ifndef SAVERESOURCE_H
#define SAVERESOURCE_H

#include <string>
#include <tuple>

namespace Game {
struct GameBoard;

namespace Saver {
void saveGamePlayState(GameBoard gb, const std::string& filename);

} // namespace Saver
} // namespace Game

#endif
