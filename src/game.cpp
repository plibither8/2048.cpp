#include "game.hpp"
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
#include <fstream>
#include <iomanip>
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

void drawScoreBoard(std::ostream &os) {
  constexpr auto score_text_label = "SCORE:";
  constexpr auto bestscore_text_label = "BEST SCORE:";
  constexpr auto moves_text_label = "MOVES:";

  // * border padding: vvv
  // | l-outer: 2, r-outer: 0
  // | l-inner: 1, r-inner: 1
  // * top border / bottom border: vvv
  // | tl_corner + horizontal_sep + tr_corner = length: 1 + 27 + 1
  // | bl_corner + horizontal_sep + br_corner = length: 1 + 27 + 1
  enum {
    UI_SCOREBOARD_SIZE = 27,
    UI_BORDER_OUTER_PADDING = 2,
    UI_BORDER_INNER_PADDING = 1
  }; // length of horizontal board - (corners + border padding)
  constexpr auto border_padding_char = ' ';
  constexpr auto vertical_border_pattern = "│";
  constexpr auto top_board =
      "┌───────────────────────────┐"; // Multibyte character set
  constexpr auto bottom_board =
      "└───────────────────────────┘"; // Multibyte character set
  const auto outer_border_padding =
      std::string(UI_BORDER_OUTER_PADDING, border_padding_char);
  const auto inner_border_padding =
      std::string(UI_BORDER_INNER_PADDING, border_padding_char);
  const auto inner_padding_length =
      UI_SCOREBOARD_SIZE - (std::string{inner_border_padding}.length() * 2);
  os << outer_border_padding << top_board << "\n";
  os << outer_border_padding << vertical_border_pattern << inner_border_padding
     << bold_on << score_text_label << bold_off
     << std::string(inner_padding_length -
                        std::string{score_text_label}.length() -
                        std::to_string(gamePlayBoard.score).length(),
                    border_padding_char)
     << gamePlayBoard.score << inner_border_padding << vertical_border_pattern
     << "\n";
  if (gamePlayBoard.getPlaySize() == COMPETITION_GAME_BOARD_PLAY_SIZE) {
    const auto tempBestScore =
        (bestScore < gamePlayBoard.score ? gamePlayBoard.score : bestScore);
    os << outer_border_padding << vertical_border_pattern
       << inner_border_padding << bold_on << bestscore_text_label << bold_off
       << std::string(inner_padding_length -
                          std::string{bestscore_text_label}.length() -
                          std::to_string(tempBestScore).length(),
                      border_padding_char)
       << tempBestScore << inner_border_padding << vertical_border_pattern
       << "\n";
  }
  os << outer_border_padding << vertical_border_pattern << inner_border_padding
     << bold_on << moves_text_label << bold_off
     << std::string(inner_padding_length -
                        std::string{moves_text_label}.length() -
                        std::to_string(gamePlayBoard.MoveCount()).length(),
                    border_padding_char)
     << gamePlayBoard.MoveCount() << inner_border_padding
     << vertical_border_pattern << "\n";
  os << outer_border_padding << bottom_board << "\n \n";
}

void drawBoard(std::ostream &os) {
  clearScreen();
  drawAscii();
  drawScoreBoard(os);
  os << gamePlayBoard;
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
  gamePlayBoard.unblockTiles();
  if (gamePlayBoard.moved) {
    gamePlayBoard.addTile();
    gamePlayBoard.registerMoveByOne();
  }

  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (gamePlayBoard.hasWon()) {
      gamestatus[FLAG_WIN] = true;
      gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = true;
    }
  }
  if (!gamePlayBoard.canMove()) {
    gamestatus[FLAG_END_GAME] = true;
  }
  return gamestatus;
}

gamestatus_t drawGraphics(std::ostream &os, gamestatus_t gamestatus) {
  drawBoard(os);
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
    gamePlayBoard.tumbleTilesUp();
    break;

  case DOWN:
    gamePlayBoard.tumbleTilesDown();
    break;

  case LEFT:
    gamePlayBoard.tumbleTilesLeft();
    break;

  case RIGHT:
    gamePlayBoard.tumbleTilesRight();
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

void drawEndScreen(std::ostream &os, gamestatus_t gamestatus) {
  const auto standardWinLosePrompt = [&gamestatus] {
    std::ostringstream str_os;
    DrawOnlyWhen(str_os, gamestatus[FLAG_WIN], Graphics::YouWinPrompt);
    // else..
    DrawOnlyWhen(str_os, !gamestatus[FLAG_WIN], Graphics::GameOverPrompt);
    return str_os.str();
  };
  DrawOnlyWhen(os, !gamestatus[FLAG_ENDLESS_MODE], standardWinLosePrompt);
  // else..
  DrawOnlyWhen(os, gamestatus[FLAG_ENDLESS_MODE], Graphics::EndOfEndlessPrompt);
}

void endlessGameLoop() {
  auto loop_again{true};
  gamestatus_t world_gamestatus{};

  while (loop_again) {
    std::tie(loop_again, world_gamestatus) = soloGameLoop(world_gamestatus);
  }

  drawBoard(std::cout);
  drawEndScreen(std::cout, world_gamestatus);
}

void drawEndGameStatistics(std::ostream &os, Scoreboard::Score finalscore) {
  constexpr auto stats_title_text = "STATISTICS";
  constexpr auto divider_text = "──────────";
  const auto stats_attributes_text = {
      "Final score:", "Largest Tile:", "Number of moves:", "Time taken:"};
  constexpr auto num_of_stats_attributes_text = 4;
  constexpr auto sp = "  ";

  auto data_stats = std::array<std::string, num_of_stats_attributes_text>{};
  data_stats = {
      std::to_string(finalscore.score), std::to_string(finalscore.largestTile),
      std::to_string(finalscore.moveCount), secondsFormat(finalscore.duration)};

  std::ostringstream stats_richtext;
  stats_richtext << yellow << sp << stats_title_text << def << "\n";
  stats_richtext << yellow << sp << divider_text << def << "\n";

  auto counter{0};
  const auto populate_stats_info = [data_stats, stats_attributes_text, &counter,
                                    &stats_richtext](const std::string) {
    stats_richtext << sp << std::left << std::setw(19)
                   << std::begin(stats_attributes_text)[counter] << bold_on
                   << std::begin(data_stats)[counter] << bold_off << "\n";
    counter++;
  };
  std::for_each(std::begin(stats_attributes_text),
                std::end(stats_attributes_text), populate_stats_info);

  os << stats_richtext.str();
  os << "\n\n";
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
  if (gamePlayBoard.getPlaySize() == COMPETITION_GAME_BOARD_PLAY_SIZE) {
    Scoreboard::Score finalscore{};
    finalscore.score = gamePlayBoard.score;
    finalscore.win = gamePlayBoard.hasWon();
    finalscore.moveCount = gamePlayBoard.MoveCount();
    finalscore.largestTile = gamePlayBoard.largestTile;
    finalscore.duration = duration;

    drawEndGameStatistics(std::cout, finalscore);
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
    gamePlayBoard.addTile();
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
