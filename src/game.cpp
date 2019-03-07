#include "game.hpp"
#include "game-graphics.hpp"
#include "game-input.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include "loadresource.hpp"
#include "point2d.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
enum Directions { UP, DOWN, RIGHT, LEFT };

enum ContinueStatus { STATUS_END_GAME = 0, STATUS_CONTINUE = 1 };

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
gamestatus_t gamestatus{};

ull bestScore;
double duration;
GameBoard gamePlayBoard;
RandInt randInt;

template<typename T>
void DrawAlways(std::ostream &os, T f) {
  os << f();
}

template<typename T>
void DrawOnlyWhen(std::ostream &os, bool trigger, T f) {
  if (trigger) {
    DrawAlways(os, f);
  }
}

template<typename T>
void DrawAsOneTimeFlag(std::ostream &os, bool &trigger, T f) {
  if (trigger) {
    DrawAlways(os, f);
    trigger = !trigger;
  }
}

std::string receive_input_player_name(std::istream &is) {
  std::string name;
  is >> name;
  return name;
}

void load_game_best_score() {
  using namespace Statistics;
  total_game_stats_t stats;
  bool stats_file_loaded{};
  std::tie(stats_file_loaded, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  if (stats_file_loaded) {
    bestScore = stats.bestScore;
  }
}

bool initialiseContinueBoardArray() {
  using namespace Game::Loader;
  constexpr auto gameboard_data_filename = "../data/previousGame";
  constexpr auto game_stats_data_filename = "../data/previousGameStats";
  auto loaded_gameboard{false};
  auto loaded_game_stats{false};
  // Note: Reserved for gameboard.score and gameboard.moveCount!
  // TODO: Combine data into one resource file.
  auto score_and_movecount = std::tuple<decltype(gamePlayBoard.score),
                                        decltype(gamePlayBoard.moveCount)>{};
  std::tie(loaded_gameboard, gamePlayBoard) =
      load_GameBoard_data_from_file(gameboard_data_filename);
  std::tie(loaded_game_stats, score_and_movecount) =
      load_game_stats_from_file(game_stats_data_filename);
  std::tie(gamePlayBoard.score, gamePlayBoard.moveCount) = score_and_movecount;

  const auto all_files_loaded_ok = (loaded_gameboard && loaded_game_stats);

  return all_files_loaded_ok;
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

void drawInputControls(std::ostream &os) {
  const auto InputControlLists = [] {
    std::ostringstream str_os;
    DrawAlways(str_os, Game::Graphics::InputCommandListPrompt);
    DrawOnlyWhen(str_os, gamestatus[FLAG_ENDLESS_MODE],
                 Game::Graphics::EndlessModeCommandListPrompt);
    DrawAlways(str_os, Game::Graphics::InputCommandListFooterPrompt);
    return str_os.str();
  };
  // When game is paused to ask a question, hide regular inut prompts..
  DrawOnlyWhen(os, !gamestatus[FLAG_QUESTION_STAY_OR_QUIT], InputControlLists);
}

bool check_input_other(char c) {
  using namespace Game::Input::Keypress::Code;
  switch (toupper(c)) {
  case CODE_HOTKEY_ACTION_SAVE:
  case CODE_HOTKEY_ALTERNATE_ACTION_SAVE:
    gamestatus[FLAG_SAVED_GAME] = true;
    return false;
  case CODE_HOTKEY_QUIT_ENDLESS_MODE:
    if (gamestatus[FLAG_ENDLESS_MODE]) {
      gamestatus[FLAG_END_GAME] = true;
      return false;
    }
    break;
  }
  return true;
}

void receive_agent_input(Game::Input::intendedmove_t &intendedmove) {
  using namespace Game::Input;
  const bool game_still_in_play =
      !gamestatus[FLAG_END_GAME] && !gamestatus[FLAG_WIN];
  if (game_still_in_play) {
    // Game still in play. Take input commands for next turn.
    char c;
    getKeypressDownInput(c);
    // Update agent's intended move flags per control scheme (if flagged).
    const auto is_invalid_keypress_code = check_input_ansi(c, intendedmove) &&
                                          check_input_wasd(c, intendedmove) &&
                                          check_input_vim(c, intendedmove) &&
                                          check_input_other(c);
    if (is_invalid_keypress_code) {
      gamestatus[FLAG_INPUT_ERROR] = true;
    }
  }
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

void drawEndGameStatistics(std::ostream &os) {
  constexpr auto stats_title_text = "STATISTICS";
  constexpr auto divider_text = "──────────";
  const auto stats_attributes_text = {
      "Final score:", "Largest Tile:", "Number of moves:", "Time taken:"};
  constexpr auto num_of_stats_attributes_text = 4;
  constexpr auto sp = "  ";

  auto data_stats = std::array<std::string, num_of_stats_attributes_text>{};
  data_stats = {std::to_string(gamePlayBoard.score),
                std::to_string(gamePlayBoard.largestTile),
                std::to_string(gamePlayBoard.MoveCount()),
                secondsFormat(duration)};

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
}

void saveEndGameStats() {
  using namespace Statistics;
  total_game_stats_t stats;
  // Need some sort of stats data values only.
  // No need to care if file loaded successfully or not...
  std::tie(std::ignore, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  stats.bestScore = stats.bestScore < gamePlayBoard.score ?
                        gamePlayBoard.score :
                        stats.bestScore;
  stats.gameCount++;
  stats.winCount = gamePlayBoard.hasWon() ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += gamePlayBoard.MoveCount();
  stats.totalDuration += duration;

  saveToFileEndGameStatistics("../data/statistics.txt", stats);
}

void saveScore() {
  Scoreboard::Score tempscore{};
  auto name = receive_input_player_name(std::cin);

  tempscore.name = name;
  tempscore.score = gamePlayBoard.score;
  tempscore.win = gamePlayBoard.hasWon();
  tempscore.moveCount = gamePlayBoard.MoveCount();
  tempscore.largestTile = gamePlayBoard.largestTile;
  tempscore.duration = duration;
  Scoreboard::saveToFileScore("../data/scores.txt", tempscore);
}

bool generateFilefromPreviousGameStatisticsData(std::ostream &os) {
  os << gamePlayBoard.score << ":" << gamePlayBoard.MoveCount();
  return true;
}

void saveToFilePreviousGameStatisticsData(std::string filename) {
  std::ofstream stats(filename, std::ios_base::app);
  generateFilefromPreviousGameStatisticsData(stats);
}

bool generateFilefromPreviousGameStateData(std::ostream &os) {
  os << gamePlayBoard.printState();
  return true;
}

void saveToFilePreviousGameStateData(std::string filename) {
  std::ofstream stateFile(filename, std::ios_base::app);
  generateFilefromPreviousGameStateData(stateFile);
}

void saveGamePlayState() {
  // Currently two datafiles for now.
  // Will be merged into one datafile in a future PR.
  std::remove("../data/previousGame");
  std::remove("../data/previousGameStats");

  saveToFilePreviousGameStateData("../data/previousGame");
  saveToFilePreviousGameStatisticsData("../data/previousGameStats");
}

void drawEndScreen(std::ostream &os) {
  const auto standardWinLosePrompt = [] {
    std::ostringstream str_os;
    DrawOnlyWhen(str_os, gamestatus[FLAG_WIN], Game::Graphics::YouWinPrompt);
    // else..
    DrawOnlyWhen(str_os, !gamestatus[FLAG_WIN], Game::Graphics::GameOverPrompt);
    return str_os.str();
  };
  DrawOnlyWhen(os, !gamestatus[FLAG_ENDLESS_MODE], standardWinLosePrompt);
  // else..
  DrawOnlyWhen(os, gamestatus[FLAG_ENDLESS_MODE],
               Game::Graphics::EndOfEndlessPrompt);
}

void drawGraphics(std::ostream &os) {
  drawBoard(os);
  DrawAsOneTimeFlag(os, gamestatus[FLAG_SAVED_GAME],
                    Game::Graphics::GameStateNowSavedPrompt);
  DrawOnlyWhen(os, gamestatus[FLAG_QUESTION_STAY_OR_QUIT],
               Game::Graphics::QuestionEndOfWinningGamePrompt);
  drawInputControls(os);
  DrawAsOneTimeFlag(os, gamestatus[FLAG_INPUT_ERROR],
                    Game::Graphics::InvalidInputGameBoardErrorPrompt);
}

void process_gamelogic() {
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
}

bool process_agent_input(Game::Input::intendedmove_t &intendedmove) {
  using namespace Game::Input;
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
  using namespace Game::Input::Keypress::Code;
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

bool process_gameStatus() {
  if (!gamestatus[FLAG_ENDLESS_MODE]) {
    if (gamestatus[FLAG_WIN]) {
      if (continue_playing_game(std::cin)) {
        gamestatus[FLAG_ENDLESS_MODE] = true;
        gamestatus[FLAG_QUESTION_STAY_OR_QUIT] = false;
        gamestatus[FLAG_WIN] = false;
      } else {
        return false;
      }
    }
  }
  if (gamestatus[FLAG_END_GAME]) {
    // End endless_mode;
    return false;
  }
  if (gamestatus[FLAG_SAVED_GAME]) {
    saveGamePlayState();
  }
  return true;
}

bool soloGameLoop() {
  using namespace Game::Input;
  intendedmove_t player_intendedmove{};
  process_gamelogic();
  drawGraphics(std::cout);
  receive_agent_input(player_intendedmove);
  process_agent_input(player_intendedmove);
  const auto loop_again = process_gameStatus();
  return loop_again;
}

void endlessGameLoop() {
  while (soloGameLoop())
    ;
}

void playGame(ContinueStatus cont) {
  load_game_best_score();
  auto startTime = std::chrono::high_resolution_clock::now();
  endlessGameLoop();
  auto finishTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finishTime - startTime;
  duration = elapsed.count();

  drawBoard(std::cout);
  drawEndScreen(std::cout);

  if (gamePlayBoard.getPlaySize() == COMPETITION_GAME_BOARD_PLAY_SIZE &&
      cont == ContinueStatus::STATUS_END_GAME) {
    drawEndGameStatistics(std::cout);
    saveEndGameStats();
    newline(2);
    DrawAlways(std::cout, Game::Graphics::AskForPlayerNamePrompt);
    saveScore();
    DrawAlways(std::cout, Game::Graphics::MessageScoreSavedPrompt);
  }
}

ull Receive_Input_Playsize(std::istream &is) {
  ull userInput_PlaySize{0};
  is >> userInput_PlaySize;
  is.clear();
  is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  return userInput_PlaySize;
}

enum class NewGameFlag { NewGameFlagNull, NoPreviousSaveAvailable };

ull askWhatIsBoardSize(std::ostream &os, NewGameFlag ns) {
  bool noSave = (ns == NewGameFlag::NoPreviousSaveAvailable) ? true : false;
  bool invalidInputValue = false;
  ull userInput_PlaySize{0};

  const auto QuestionAboutBoardSizePrompt = [&invalidInputValue, &noSave]() {
    std::ostringstream str_os;
    clearScreen();
    drawAscii();
    // Prints only if "invalidInputValue" is true
    DrawOnlyWhen(str_os, invalidInputValue,
                 Game::Graphics::BoardSizeErrorPrompt);
    // Prints only if "noSave" is true
    // Consumes "noSave" flag (turns flag to false/off)
    DrawAsOneTimeFlag(str_os, noSave,
                      Game::Graphics::GameBoardNoSaveErrorPrompt);
    DrawAlways(str_os, Game::Graphics::BoardInputPrompt);
    return str_os.str();
  };

  while ((userInput_PlaySize < MIN_GAME_BOARD_PLAY_SIZE) ||
         (userInput_PlaySize > MAX_GAME_BOARD_PLAY_SIZE)) {
    DrawAlways(os, QuestionAboutBoardSizePrompt);
    userInput_PlaySize = Receive_Input_Playsize(std::cin);
    invalidInputValue = true;
  }
  return userInput_PlaySize;
}

void SetUpNewGame(NewGameFlag ns = NewGameFlag::NewGameFlagNull) {
  ull userInput_PlaySize = askWhatIsBoardSize(std::cout, ns);
  gamePlayBoard = GameBoard(userInput_PlaySize);
  gamePlayBoard.addTile();
  playGame(ContinueStatus::STATUS_END_GAME);
}

} // namespace

void Game::startGame() {
  SetUpNewGame();
}

void Game::continueGame() {
  if (initialiseContinueBoardArray()) {
    playGame(ContinueStatus::STATUS_CONTINUE);
  } else {
    SetUpNewGame(NewGameFlag::NoPreviousSaveAvailable);
  }
}
