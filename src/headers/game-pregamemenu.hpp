#ifndef GAMEPREGAMEMENU_H
#define GAMEPREGAMEMENU_H

#include <string>

namespace Game {
namespace PreGameSetup {
    
void SetUpNewGame();
void ContinueOldGame(const std::string& filename);

} // namespace PreGameSetup
} // namespace Game

#endif
