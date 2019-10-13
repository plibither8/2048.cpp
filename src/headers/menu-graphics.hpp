#ifndef MENUGRAPHICS_H
#define MENUGRAPHICS_H

#include <string>

namespace Game {
namespace Graphics {
namespace Menu {
std::string MainMenuTitlePrompt();
std::string MainMenuOptionsPrompt();
std::string InputMenuErrorInvalidInputPrompt();
std::string InputMenuPrompt();
std::string MainMenuGraphicsOverlay(bool input_error_choice_invalid);
} // namespace Menu
} // namespace Graphics
} // namespace Game

#endif
