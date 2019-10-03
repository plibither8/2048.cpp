#include "game.hpp"
#include "color.hpp"
#include "game-graphics.hpp"
#include "game-input.hpp"
#include "game-pregamemenu.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include "loadresource.hpp"
#include "menu.hpp"
#include "point2d.hpp"
#include "saveresource.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <algorithm>
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
  FLAG_QUESTION_STAY_OR_QUIT,
  MAX_NO_GAME_STATUS_FLAGS
};

using gamestatus_t = std::array<bool, MAX_NO_GAME_STATUS_FLAGS>;

ull bestScore;
GameBoard gamePlayBoard;

std::string receive_input_player_name(std::istream &is) {
  std::string name;
  is >> name;
  return name;
}

ull load_game_best_score() {
  using namespace Statistics;
  total_game_stats_t stats;
  bool stats_file_loaded{};
  ull tempscore{0};
  std::tie(stats_file_loaded, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  if (stats_file_loaded) {
    tempscore = stats.bestScore;
  }
  return tempscore;
}

void drawInputControls(std::ostream &os, gamestatus_t gamestatus) {
  const auto InputControlLists = [&gamestatus] {
    std::ostringstream str_os;
    DrawAlways(str_os, Graphics::InputCommandListPrompt);
    DrawOnlyWhen(str_os, gamestatus[FLAG_ENDLESS_MODE],
                 Graphics::EndlessModeCommandListPrompt);
    DrawAlways(str_os, Graphics::InputCommandListFooterPrompt);
    return str_os.str();
  };
  // When game is paused to ask a question, hide regular inut prompts..
  DrawOnlyWhen(os, !gamestatus[FLAG_QUESTION_STAY_OR_QUIT], InputControlLists);
}

gamestatus_t process_gamelogic(gamestatus_t gamestatus) {
  unblockTilesOnGameboard(gamePlayBoard);
  if (gamePlayBoard.moved) {
    addTileOnGameboard(gamePlayBoard);
    registerMoveByOneOnGameboard(gamePlayBoard);
  }

  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (hasWonOnGameboard(gamePlayBoard)) {
      gamestatus[FLAG_WIN] = true;
      gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = true;
    }
  }
  if (!canMoveOnGameboard(gamePlayBoard)) {
    gamestatus[FLAG_END_GAME] = true;
  }
  return gamestatus;
}

gamestatus_t drawGraphics(std::ostream &os, gamestatus_t gamestatus) {
  const auto gameboard_score = gamePlayBoard.score;
  const auto tempBestScore =
      (bestScore < gamePlayBoard.score ? gamePlayBoard.score : bestScore);
  const auto comp_mode =
      std::get<0>(gamePlayBoard.gbda) == COMPETITION_GAME_BOARD_PLAY_SIZE;
  const auto movecount = MoveCountOnGameBoard(gamePlayBoard);
  const auto scdd =
      std::make_tuple(comp_mode, std::to_string(gameboard_score),
                      std::to_string(tempBestScore), std::to_string(movecount));
  clearScreen();
  DrawAlways(os, DataSuppliment(scdd, Graphics::GameScoreBoardOverlay));
  os << gamePlayBoard;
  DrawAsOneTimeFlag(os, gamestatus[FLAG_SAVED_GAME],
                    Graphics::GameStateNowSavedPrompt);
  DrawOnlyWhen(os, gamestatus[FLAG_QUESTION_STAY_OR_QUIT],
               Graphics::QuestionEndOfWinningGamePrompt);
  drawInputControls(os, gamestatus);
  DrawAsOneTimeFlag(os, gamestatus[FLAG_INPUT_ERROR],
                    Graphics::InvalidInputGameBoardErrorPrompt);
  return gamestatus;
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

void decideMove(Directions d) {
  switch (d) {
  case UP:
    tumbleTilesUpOnGameboard(gamePlayBoard);
    break;

  case DOWN:
    tumbleTilesDownOnGameboard(gamePlayBoard);
    break;

  case LEFT:
    tumbleTilesLeftOnGameboard(gamePlayBoard);
    break;

  case RIGHT:
    tumbleTilesRightOnGameboard(gamePlayBoard);
    break;
  }
}

bool process_agent_input(Input::intendedmove_t &intendedmove) {
  using namespace Input;
  if (intendedmove[FLAG_MOVE_LEFT]) {
    decideMove(LEFT);
  }
  if (intendedmove[FLAG_MOVE_RIGHT]) {
    decideMove(RIGHT);
  }
  if (intendedmove[FLAG_MOVE_UP]) {
    decideMove(UP);
  }
  if (intendedmove[FLAG_MOVE_DOWN]) {
    decideMove(DOWN);
  }
  // Reset all move flags...
  intendedmove = intendedmove_t{};
  return true;
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

void saveGamePlayState() {
  using namespace Saver;
  // Currently two datafiles for now.
  // Will be merged into one datafile in a future PR.
  std::remove("../data/previousGame");
  std::remove("../data/previousGameStats");

  saveToFilePreviousGameStateData("../data/previousGame", gamePlayBoard);
  saveToFilePreviousGameStatisticsData("../data/previousGameStats",
                                       gamePlayBoard);
}

wrapper_bool_gamestatus_t process_gameStatus(gamestatus_t gamestatus) {
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
    saveGamePlayState();
  }
  return std::make_tuple(loop_again, gamestatus);
}

wrapper_bool_gamestatus_t soloGameLoop(gamestatus_t gamestatus) {
  using namespace Input;
  intendedmove_t player_intendedmove{};
  gamestatus = process_gamelogic(gamestatus);
  gamestatus = drawGraphics(std::cout, gamestatus);
  gamestatus = receive_agent_input(player_intendedmove, gamestatus);
  process_agent_input(player_intendedmove);
  const auto loop_again = process_gameStatus(gamestatus);
  return loop_again;
}

void endlessGameLoop() {
  auto loop_again{true};
  gamestatus_t world_gamestatus{};

  while (loop_again) {
    std::tie(loop_again, world_gamestatus) = soloGameLoop(world_gamestatus);
  }

  const auto gameboard_score = gamePlayBoard.score;
  const auto tempBestScore =
      (bestScore < gamePlayBoard.score ? gamePlayBoard.score : bestScore);
  const auto comp_mode =
      std::get<0>(gamePlayBoard.gbda) == COMPETITION_GAME_BOARD_PLAY_SIZE;
  const auto movecount = MoveCountOnGameBoard(gamePlayBoard);
  const auto scdd =
      std::make_tuple(comp_mode, std::to_string(gameboard_score),
                      std::to_string(tempBestScore), std::to_string(movecount));
  clearScreen();
  DrawAlways(std::cout, DataSuppliment(scdd, Graphics::GameScoreBoardOverlay));
  std::cout << gamePlayBoard;
  const auto esdd = std::make_tuple(world_gamestatus[FLAG_WIN],
                                    world_gamestatus[FLAG_ENDLESS_MODE]);
  DrawAlways(std::cout, DataSuppliment(esdd, Graphics::drawEndScreen));
}

void saveEndGameStats(Scoreboard::Score finalscore) {
  using namespace Statistics;
  total_game_stats_t stats;
  // Need some sort of stats data values only.
  // No need to care if file loaded successfully or not...
  std::tie(std::ignore, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  stats.bestScore =
      stats.bestScore < finalscore.score ? finalscore.score : stats.bestScore;
  stats.gameCount++;
  stats.winCount = finalscore.win ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += finalscore.moveCount;
  stats.totalDuration += finalscore.duration;

  saveToFileEndGameStatistics("../data/statistics.txt", stats);
}

void saveScore(Scoreboard::Score finalscore) {
  Scoreboard::saveToFileScore("../data/scores.txt", finalscore);
}

void DoPostGameSaveStuff(double duration) {
  if (std::get<0>(gamePlayBoard.gbda) == COMPETITION_GAME_BOARD_PLAY_SIZE) {
    Scoreboard::Score finalscore{};
    finalscore.score = gamePlayBoard.score;
    finalscore.win = hasWonOnGameboard(gamePlayBoard);
    finalscore.moveCount = MoveCountOnGameBoard(gamePlayBoard);
    finalscore.largestTile = gamePlayBoard.largestTile;
    finalscore.duration = duration;

    const auto finalscore_display_data =
        std::make_tuple(std::to_string(finalscore.score),
                        std::to_string(finalscore.largestTile),
                        std::to_string(finalscore.moveCount),
                        secondsFormat(finalscore.duration));

    DrawAlways(std::cout, DataSuppliment(finalscore_display_data,
                                         Graphics::EndGameStatisticsPrompt));
    saveEndGameStats(finalscore);

    DrawAlways(std::cout, Graphics::AskForPlayerNamePrompt);
    const auto name = receive_input_player_name(std::cin);
    finalscore.name = name;

    saveScore(finalscore);
    DrawAlways(std::cout, Graphics::MessageScoreSavedPrompt);
  }
}

} // namespace

void playGame(PlayGameFlag cont, GameBoard gb, ull userInput_PlaySize) {
  bestScore = load_game_best_score();
  gamePlayBoard = gb;
  if (cont == PlayGameFlag::BrandNewGame) {
    gamePlayBoard = GameBoard(userInput_PlaySize);
    addTileOnGameboard(gamePlayBoard);
  }

  const auto startTime = std::chrono::high_resolution_clock::now();
  endlessGameLoop();
  const auto finishTime = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> elapsed = finishTime - startTime;
  const auto duration = elapsed.count();

  if (cont == PlayGameFlag::BrandNewGame) {
    DoPostGameSaveStuff(duration);
  }
}

void startGame() {
  PreGameSetup::SetUpNewGame();
}

void continueGame() {
  PreGameSetup::ContinueOldGame();
}

} // namespace Game
