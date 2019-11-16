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
  FLAG_ONE_SHOT,
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

using competition_mode_t = bool;
Graphics::scoreboard_display_data_t
make_scoreboard_display_data(ull bestScore, competition_mode_t cm,
                             GameBoard gb) {
  const auto gameboard_score = gb.score;
  const auto tempBestScore = (bestScore < gb.score ? gb.score : bestScore);
  const auto comp_mode = cm;
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

// NOTE: current_game_session_t
// : (bestScore, is_competition_mode, gamestatus, gamePlayBoard)
using current_game_session_t =
    std::tuple<ull, competition_mode_t, gamestatus_t, GameBoard>;
enum tuple_cgs_t_idx {
  IDX_BESTSCORE,
  IDX_COMP_MODE,
  IDX_GAMESTATUS,
  IDX_GAMEBOARD
};

std::string drawGraphics(current_game_session_t cgs) {
  // Graphical Output has a specific ordering...
  using namespace Graphics;
  using namespace Gameboard::Graphics;
  using tup_idx = tuple_cgs_t_idx;
  const auto bestScore = std::get<tup_idx::IDX_BESTSCORE>(cgs);
  const auto comp_mode = std::get<tup_idx::IDX_COMP_MODE>(cgs);
  const auto gb = std::get<tup_idx::IDX_GAMEBOARD>(cgs);
  const auto gamestatus = std::get<tup_idx::IDX_GAMESTATUS>(cgs);

  std::ostringstream str_os;

  // 1. Clear screen
  clearScreen();

  // 2. Draw Game Title Art
  DrawAlways(str_os, AsciiArt2048);

  // 3. Draw Scoreboard of current game session
  const auto scdd = make_scoreboard_display_data(bestScore, comp_mode, gb);
  DrawAlways(str_os, DataSuppliment(scdd, GameScoreBoardOverlay));

  // 4. Draw current 2048 game active gameboard
  DrawAlways(str_os, DataSuppliment(gb, GameBoardTextOutput));

  // 5. Draw anyinstant status feedback, like
  // "Game saved!" (which disappers after next key input).
  DrawOnlyWhen(str_os, gamestatus[FLAG_SAVED_GAME], GameStateNowSavedPrompt);

  // 6. Draw any "questions to the player" (from the game) text output
  DrawOnlyWhen(str_os, gamestatus[FLAG_GAME_IS_ASKING_QUESTION_MODE],
               DataSuppliment(gamestatus, DisplayGameQuestionsToPlayerPrompt));

  // 7. Draw Keyboard / Input Keycodes to the player
  const auto input_controls_display_data =
      make_input_controls_display_data(gamestatus);
  DrawAlways(str_os, DataSuppliment(input_controls_display_data,
                                    GameInputControlsOverlay));

  // 8. Draw any game error messages to the player (to do with keyboard input)
  DrawOnlyWhen(str_os, gamestatus[FLAG_INPUT_ERROR],
               InvalidInputGameBoardErrorPrompt);

  return str_os.str();
}

gamestatus_t update_one_shot_display_flags(gamestatus_t gamestatus) {
  const auto disable_one_shot_flag = [](bool &trigger) { trigger = !trigger; };
  if (gamestatus[FLAG_ONE_SHOT]) {
    disable_one_shot_flag(gamestatus[FLAG_ONE_SHOT]);
    // Turn off display flag: [Saved Game]
    if (gamestatus[FLAG_SAVED_GAME]) {
      disable_one_shot_flag(gamestatus[FLAG_SAVED_GAME]);
    }
    // Turn off display flag: [Input Error]
    if (gamestatus[FLAG_INPUT_ERROR]) {
      disable_one_shot_flag(gamestatus[FLAG_INPUT_ERROR]);
    }
  }
  return gamestatus;
}

using bool_gamestatus_t = std::tuple<bool, gamestatus_t>;
bool_gamestatus_t check_input_other(char c, gamestatus_t gamestatus) {
  using namespace Input::Keypress::Code;
  auto is_invalid_keycode{true};
  switch (toupper(c)) {
  case CODE_HOTKEY_ACTION_SAVE:
  case CODE_HOTKEY_ALTERNATE_ACTION_SAVE:
    gamestatus[FLAG_ONE_SHOT] = true;
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
using intendedmove_gamestatus_t =
    std::tuple<Input::intendedmove_t, gamestatus_t>;
intendedmove_gamestatus_t
receive_agent_input(Input::intendedmove_t intendedmove,
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
      gamestatus[FLAG_ONE_SHOT] = true;
      gamestatus[FLAG_INPUT_ERROR] = true;
    }
  }
  return std::make_tuple(intendedmove, gamestatus);
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
bool_gameboard_t process_agent_input(Input::intendedmove_t intendedmove,
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

bool_gamestatus_t process_gameStatus(gamestatus_gameboard_t gsgb) {
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
  using tup_idx = tuple_cgs_t_idx;
  const auto gamestatus =
      std::addressof(std::get<tup_idx::IDX_GAMESTATUS>(cgs));
  const auto gb = std::addressof(std::get<tup_idx::IDX_GAMEBOARD>(cgs));

  std::tie(*gamestatus, *gb) =
      process_gamelogic(std::make_tuple(*gamestatus, *gb));

  DrawAlways(std::cout, DataSuppliment(cgs, drawGraphics));
  *gamestatus = update_one_shot_display_flags(*gamestatus);

  intendedmove_t player_intendedmove{};
  std::tie(player_intendedmove, *gamestatus) =
      receive_agent_input(player_intendedmove, *gamestatus);
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
  using tup_idx = tuple_cgs_t_idx;
  const auto bestScore = std::get<tup_idx::IDX_BESTSCORE>(finalgamestatus);
  const auto comp_mode = std::get<tup_idx::IDX_COMP_MODE>(finalgamestatus);
  const auto gb = std::get<tup_idx::IDX_GAMEBOARD>(finalgamestatus);
  const auto end_gamestatus =
      std::get<tup_idx::IDX_GAMESTATUS>(finalgamestatus);

  std::ostringstream str_os;

  // 1. Clear screen
  clearScreen();

  // 2. Draw Game Title Art
  DrawAlways(str_os, AsciiArt2048);

  // 3. Draw Scoreboard of ending current game session
  const auto scdd = make_scoreboard_display_data(bestScore, comp_mode, gb);
  DrawAlways(str_os, DataSuppliment(scdd, GameScoreBoardOverlay));

  // 4. Draw snapshot of ending 2048 session's gameboard
  DrawAlways(str_os, DataSuppliment(gb, GameBoardTextOutput));

  // 5. Draw "You win!" or "You Lose" prompt, only if not in endless mode.
  const auto esdd = make_end_screen_display_data(end_gamestatus);
  DrawAlways(str_os, DataSuppliment(esdd, GameEndScreenOverlay));

  return str_os.str();
}

GameBoard endlessGameLoop(ull currentBestScore, competition_mode_t cm,
                          GameBoard gb) {
  auto loop_again{true};
  auto currentgamestatus =
      std::make_tuple(currentBestScore, cm, gamestatus_t{}, gb);

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

void DoPostGameSaveStuff(Scoreboard::Score finalscore, competition_mode_t cm) {
  if (cm) {
    Statistics::CreateFinalScoreAndEndGameDataFile(std::cout, std::cin,
                                                   finalscore);
  }
}

} // namespace

void playGame(PlayGameFlag cont, GameBoard gb, ull userInput_PlaySize) {
  const auto is_this_a_new_game = (cont == PlayGameFlag::BrandNewGame);
  const auto is_competition_mode =
      (userInput_PlaySize == COMPETITION_GAME_BOARD_PLAY_SIZE);
  const auto bestScore = Statistics::load_game_best_score();

  if (is_this_a_new_game) {
    gb = GameBoard(userInput_PlaySize);
    addTileOnGameboard(gb);
  }

  const auto startTime = std::chrono::high_resolution_clock::now();
  gb = endlessGameLoop(bestScore, is_competition_mode, gb);
  const auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed = finishTime - startTime;
  const auto duration = elapsed.count();

  if (is_this_a_new_game) {
    const auto finalscore = make_finalscore_from_game_session(duration, gb);
    DoPostGameSaveStuff(finalscore, is_competition_mode);
  }
}

void startGame() {
  PreGameSetup::SetUpNewGame();
}

void continueGame() {
  PreGameSetup::ContinueOldGame();
}

} // namespace Game
