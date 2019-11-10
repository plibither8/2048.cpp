#include "game.hpp"
#include "game-graphics.hpp"
#include "game-input.hpp"
#include "game-pregamemenu.hpp"
#include "gameboard-graphics.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include "loadresource.hpp"
#include "menu.hpp"
#include "saveresource.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <array>
#include <chrono>
#include <iostream>
#include <sstream>

namespace Game {
namespace {
enum Directions { UP, DOWN, RIGHT, LEFT };

enum GameStatusFlag {
  FLAG_WIN,
  FLAG_END_GAME,
  FLAG_SAVED_GAME,
  FLAG_INPUT_ERROR,
  FLAG_ENDLESS_MODE,
  FLAG_GAME_IS_ASKING_QUESTION_MODE,
  FLAG_QUESTION_STAY_OR_QUIT,
  MAX_NO_GAME_STATUS_FLAGS
};

using gamestatus_t = std::array<bool, MAX_NO_GAME_STATUS_FLAGS>;

using gamestatus_gameboard_t = std::tuple<gamestatus_t, GameBoard>;

gamestatus_gameboard_t process_gamelogic(gamestatus_gameboard_t gsgb) {
  gamestatus_t gamestatus;
  GameBoard gb;
  std::tie(gamestatus, gb) = gsgb;
  unblockTilesOnGameboard(gb);
  if (gb.moved) {
    addTileOnGameboard(gb);
    registerMoveByOneOnGameboard(gb);
  }

  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (hasWonOnGameboard(gb)) {
      gamestatus[FLAG_WIN] = true;
      gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE] = true;
      gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = true;
    }
  }
  if (!canMoveOnGameboard(gb)) {
    gamestatus[FLAG_END_GAME] = true;
  }
  return std::make_tuple(gamestatus, gb);
}

Graphics::scoreboard_display_data_t make_scoreboard_display_data(ull bestScore,
                                                                 GameBoard gb) {
  const auto gameboard_score = gb.score;
  const auto tempBestScore = (bestScore < gb.score ? gb.score : bestScore);
  const auto comp_mode =
      std::get<0>(gb.gbda) == COMPETITION_GAME_BOARD_PLAY_SIZE;
  const auto movecount = MoveCountOnGameBoard(gb);
  const auto scdd =
      std::make_tuple(comp_mode, std::to_string(gameboard_score),
                      std::to_string(tempBestScore), std::to_string(movecount));
  return scdd;
};

Graphics::input_controls_display_data_t
make_input_controls_display_data(gamestatus_t gamestatus) {
  const auto icdd = std::make_tuple(gamestatus[FLAG_ENDLESS_MODE],
                                    gamestatus[FLAG_QUESTION_STAY_OR_QUIT]);
  return icdd;
};

std::string DisplayGameQuestionsToPlayerPrompt(gamestatus_t gamestatus) {
  using namespace Graphics;

  std::ostringstream str_os;
  DrawOnlyWhen(str_os, gamestatus[FLAG_QUESTION_STAY_OR_QUIT],
               QuestionEndOfWinningGamePrompt);
  return str_os.str();
}

// NOTE: current_game_session_t : (bestScore, gamestatus, gamePlayBoard)
using current_game_session_t = std::tuple<ull, gamestatus_t, GameBoard>;
current_game_session_t drawGraphics(std::ostream &os,
                                    current_game_session_t cgs_gb) {
  // Graphical Output has a specific ordering...
  using namespace Graphics;
  using namespace Gameboard::Graphics;
  enum tuple_idx { IDX_BESTSCORE, IDX_GAMESTATUS, IDX_GAMEBOARD };

  const auto gb = std::get<IDX_GAMEBOARD>(cgs_gb);

  // 1. Clear screen
  clearScreen();

  // 2. Draw Game Title Art
  DrawAlways(os, AsciiArt2048);

  // 3. Draw Scoreboard of current game session
  const auto bestScore = std::get<IDX_BESTSCORE>(cgs_gb);
  const auto scdd = make_scoreboard_display_data(bestScore, gb);
  DrawAlways(os, DataSuppliment(scdd, GameScoreBoardOverlay));

  // 4. Draw current 2048 game active gameboard
  DrawAlways(os, DataSuppliment(gb, GameBoardTextOutput));

  // 5. Get the current game status flags events
  auto gamestatus = std::get<IDX_GAMESTATUS>(cgs_gb);

  // 6. Draw anyinstant status feedback, like
  // "Game saved!" (which disappers after next key input).
  DrawAsOneTimeFlag(os, gamestatus[FLAG_SAVED_GAME], GameStateNowSavedPrompt);

  // 7. Draw any "questions to the player" (from the game) text output
  DrawOnlyWhen(os, gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE],
               DataSuppliment(gamestatus, DisplayGameQuestionsToPlayerPrompt));

  // 8. Draw Keyboard / Input Keycodes to the player
  const auto input_controls_display_data =
      make_input_controls_display_data(gamestatus);
  DrawAlways(os, DataSuppliment(input_controls_display_data,
                                GameInputControlsOverlay));

  // 9. Draw any game error messages to the player (to do with keyboard input)
  DrawAsOneTimeFlag(os, gamestatus[FLAG_INPUT_ERROR],
                    InvalidInputGameBoardErrorPrompt);

  std::get<IDX_GAMESTATUS>(cgs_gb) = gamestatus;
  return cgs_gb;
}

using wrapper_bool_gamestatus_t = std::tuple<bool, gamestatus_t>;
wrapper_bool_gamestatus_t check_input_other(char c, gamestatus_t gamestatus) {
  using namespace Input::Keypress::Code;
  auto is_invalid_keycode{true};
  switch (toupper(c)) {
  case CODE_HOTKEY_ACTION_SAVE:
  case CODE_HOTKEY_ALTERNATE_ACTION_SAVE:
    gamestatus[FLAG_SAVED_GAME] = true;
    is_invalid_keycode = false;
    break;
  case CODE_HOTKEY_QUIT_ENDLESS_MODE:
    if (gamestatus[FLAG_ENDLESS_MODE]) {
      gamestatus[FLAG_END_GAME] = true;
      is_invalid_keycode = false;
    }
    break;
  }
  return std::make_tuple(is_invalid_keycode, gamestatus);
}

gamestatus_t receive_agent_input(Input::intendedmove_t &intendedmove,
                                 gamestatus_t gamestatus) {
  using namespace Input;
  const bool game_still_in_play =
      !gamestatus[FLAG_END_GAME] && !gamestatus[FLAG_WIN];
  if (game_still_in_play) {
    // Game still in play. Take input commands for next turn.
    char c;
    getKeypressDownInput(c);
    // Update agent's intended move flags per control scheme (if flagged).
    const auto is_invalid_keypress_code = check_input_ansi(c, intendedmove) &&
                                          check_input_wasd(c, intendedmove) &&
                                          check_input_vim(c, intendedmove);
    bool is_invalid_special_keypress_code;
    std::tie(is_invalid_special_keypress_code, gamestatus) =
        check_input_other(c, gamestatus);
    if (is_invalid_keypress_code && is_invalid_special_keypress_code) {
      gamestatus[FLAG_INPUT_ERROR] = true;
    }
  }
  return gamestatus;
}

GameBoard decideMove(Directions d, GameBoard gb) {
  switch (d) {
  case UP:
    tumbleTilesUpOnGameboard(gb);
    break;

  case DOWN:
    tumbleTilesDownOnGameboard(gb);
    break;

  case LEFT:
    tumbleTilesLeftOnGameboard(gb);
    break;

  case RIGHT:
    tumbleTilesRightOnGameboard(gb);
    break;
  }
  return gb;
}

using bool_gameboard_t = std::tuple<bool, GameBoard>;
bool_gameboard_t process_agent_input(Input::intendedmove_t &intendedmove,
                                     GameBoard gb) {
  using namespace Input;
  if (intendedmove[FLAG_MOVE_LEFT]) {
    gb = decideMove(LEFT, gb);
  }
  if (intendedmove[FLAG_MOVE_RIGHT]) {
    gb = decideMove(RIGHT, gb);
  }
  if (intendedmove[FLAG_MOVE_UP]) {
    gb = decideMove(UP, gb);
  }
  if (intendedmove[FLAG_MOVE_DOWN]) {
    gb = decideMove(DOWN, gb);
  }
  // Reset all move flags...
  intendedmove = intendedmove_t{};
  return std::make_tuple(true, gb);
}

bool check_input_check_to_end_game(char c) {
  using namespace Input::Keypress::Code;
  switch (std::toupper(c)) {
  case CODE_HOTKEY_CHOICE_NO:
    return true;
  }
  return false;
}

bool continue_playing_game(std::istream &in_os) {
  char letter_choice;
  in_os >> letter_choice;
  if (check_input_check_to_end_game(letter_choice)) {
    return false;
  }
  return true;
}

wrapper_bool_gamestatus_t process_gameStatus(gamestatus_gameboard_t gsgb) {
  gamestatus_t gamestatus;
  GameBoard gb;
  std::tie(gamestatus, gb) = gsgb;
  auto loop_again{true};
  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (gamestatus[FLAG_WIN]) {
      if (continue_playing_game(std::cin)) {
        gamestatus[FLAG_ENDLESS_MODE] = true;
        gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = false;
        gamestatus[FLAG_WIN] = false;
      } else {
        loop_again = false;
      }
    }
  }
  if (gamestatus[FLAG_END_GAME]) {
    // End endless_mode;
    loop_again = false;
  }
  if (gamestatus[FLAG_SAVED_GAME]) {
    Saver::saveGamePlayState(gb);
  }

  // New loop cycle: reset question asking event trigger
  gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE] = false;
  return std::make_tuple(loop_again, gamestatus);
}

using bool_current_game_session_t = std::tuple<bool, current_game_session_t>;
bool_current_game_session_t soloGameLoop(current_game_session_t cgs) {
  using namespace Input;
  enum tuple_idx { IDX_BESTSCORE, IDX_GAMESTATUS, IDX_GAMEBOARD };
  const auto gamestatus = std::addressof(std::get<IDX_GAMESTATUS>(cgs));
  const auto gb = std::addressof(std::get<IDX_GAMEBOARD>(cgs));

  std::tie(*gamestatus, *gb) =
      process_gamelogic(std::make_tuple(*gamestatus, *gb));

  cgs = drawGraphics(std::cout, cgs);

  intendedmove_t player_intendedmove{};
  *gamestatus = receive_agent_input(player_intendedmove, *gamestatus);
  std::tie(std::ignore, *gb) = process_agent_input(player_intendedmove, *gb);

  bool loop_again;
  std::tie(loop_again, *gamestatus) =
      process_gameStatus(std::make_tuple(*gamestatus, *gb));
  return std::make_tuple(loop_again, cgs);
}

Graphics::end_screen_display_data_t
make_end_screen_display_data(gamestatus_t world_gamestatus) {
  const auto esdd = std::make_tuple(world_gamestatus[FLAG_WIN],
                                    world_gamestatus[FLAG_ENDLESS_MODE]);
  return esdd;
};

std::string drawEndGameLoopGraphics(current_game_session_t finalgamestatus) {
  // Graphical Output has a specific ordering...
  using namespace Graphics;
  using namespace Gameboard::Graphics;
  enum tuple_idx { IDX_BESTSCORE, IDX_GAMESTATUS, IDX_GAMEBOARD };

  const auto gb = std::get<IDX_GAMEBOARD>(finalgamestatus);

  std::ostringstream str_os;
  // 1. Clear screen
  clearScreen();

  // 2. Draw Game Title Art
  DrawAlways(str_os, AsciiArt2048);

  // 3. Draw Scoreboard of ending current game session
  const auto bestScore = std::get<IDX_BESTSCORE>(finalgamestatus);
  const auto scdd = make_scoreboard_display_data(bestScore, gb);
  DrawAlways(str_os, DataSuppliment(scdd, GameScoreBoardOverlay));

  // 4. Draw snapshot of ending 2048 session's gameboard
  DrawAlways(str_os, DataSuppliment(gb, GameBoardTextOutput));

  // 5. Draw "You win!" or "You Lose" prompt, only if not in endless mode.
  const auto end_gamestatus = std::get<IDX_GAMESTATUS>(finalgamestatus);
  const auto esdd = make_end_screen_display_data(end_gamestatus);
  DrawAlways(str_os, DataSuppliment(esdd, GameEndScreenOverlay));

  return str_os.str();
}

GameBoard endlessGameLoop(ull currentBestScore, GameBoard gb) {
  auto loop_again{true};
  auto currentgamestatus =
      std::make_tuple(currentBestScore, gamestatus_t{}, gb);

  while (loop_again) {
    std::tie(loop_again, currentgamestatus) = soloGameLoop(currentgamestatus);
  }

  DrawAlways(std::cout,
             DataSuppliment(currentgamestatus, drawEndGameLoopGraphics));
  return gb;
}

Scoreboard::Score make_finalscore_from_game_session(double duration,
                                                    GameBoard gb) {
  Scoreboard::Score finalscore{};
  finalscore.score = gb.score;
  finalscore.win = hasWonOnGameboard(gb);
  finalscore.moveCount = MoveCountOnGameBoard(gb);
  finalscore.largestTile = gb.largestTile;
  finalscore.duration = duration;
  return finalscore;
}

void DoPostGameSaveStuff(Scoreboard::Score finalscore, GameBoard gb) {
  if (std::get<0>(gb.gbda) == COMPETITION_GAME_BOARD_PLAY_SIZE) {
    Statistics::CreateFinalScoreAndEndGameDataFile(finalscore);
  }
}

} // namespace

void playGame(PlayGameFlag cont, GameBoard gb, ull userInput_PlaySize) {
  auto bestScore = Statistics::load_game_best_score();
  if (cont == PlayGameFlag::BrandNewGame) {
    gb = GameBoard(userInput_PlaySize);
    addTileOnGameboard(gb);
  }

  const auto startTime = std::chrono::high_resolution_clock::now();
  gb = endlessGameLoop(bestScore, gb);
  const auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed = finishTime - startTime;
  const auto duration = elapsed.count();

  if (cont == PlayGameFlag::BrandNewGame) {
    const auto finalscore = make_finalscore_from_game_session(duration, gb);
    DoPostGameSaveStuff(finalscore, gb);
  }
}

void startGame() {
  PreGameSetup::SetUpNewGame();
}

void continueGame() {
  PreGameSetup::ContinueOldGame();
}

} // namespace Game
