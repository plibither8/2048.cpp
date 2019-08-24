#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <string>

enum GameBoardDimensions {
  MIN_GAME_BOARD_PLAY_SIZE = 3,
  MAX_GAME_BOARD_PLAY_SIZE = 10
};
enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

namespace Game {
namespace Graphics {
std::string MessageScoreSavedPrompt();
std::string AskForPlayerNamePrompt();
std::string BoardInputPrompt();
std::string YouWinPrompt();
std::string GameOverPrompt();
std::string EndOfEndlessPrompt();
std::string InvalidInputGameBoardErrorPrompt();
std::string QuestionEndOfWinningGamePrompt();
std::string GameStateNowSavedPrompt();
std::string GameBoardNoSaveErrorPrompt();
std::string BoardSizeErrorPrompt();
std::string InputCommandListPrompt();
std::string EndlessModeCommandListPrompt();
std::string InputCommandListFooterPrompt();
namespace Menu {
std::string MainMenuTitlePrompt();
std::string MainMenuOptionsPrompt();
std::string InputMenuErrorInvalidInputPrompt();
std::string InputMenuPrompt();
} // namespace Menu
} // namespace Graphics
} // namespace Game

#endif
