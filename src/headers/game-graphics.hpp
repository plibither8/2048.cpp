#ifndef GAMEGRAPHICS_H
#define GAMEGRAPHICS_H

#include <string>
#include <tuple>

enum GameBoardDimensions {
  MIN_GAME_BOARD_PLAY_SIZE = 3,
  MAX_GAME_BOARD_PLAY_SIZE = 10
};
enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

namespace Game {
namespace Graphics {
std::string AsciiArt2048();
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
using scoreboard_display_data_t =
    std::tuple<bool, std::string, std::string, std::string>;
std::string GameScoreBoardBox(scoreboard_display_data_t scdd);
std::string GameScoreBoardOverlay(scoreboard_display_data_t scdd);
using end_screen_display_data_t = std::tuple<bool, bool>;
std::string GameEndScreenOverlay(end_screen_display_data_t esdd);
using input_controls_display_data_t = std::tuple<bool, bool>;
std::string GameInputControlsOverlay(input_controls_display_data_t gamestatus);
} // namespace Graphics
} // namespace Game

#endif
