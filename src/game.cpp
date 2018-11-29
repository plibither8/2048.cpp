#include "game.hpp"
#include "menu.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>

namespace {
namespace Keypress {
namespace Code {

enum { CODE_ESC = 27, CODE_LSQUAREBRACKET = '[' };

// Hotkey bindings:
// Style: ANSI (Arrow Keys)
enum {
  CODE_ANSI_TRIGGER_1 = CODE_ESC,
  CODE_ANSI_TRIGGER_2 = CODE_LSQUAREBRACKET
};
enum {
  CODE_ANSI_UP = 'A',
  CODE_ANSI_DOWN = 'B',
  CODE_ANSI_LEFT = 'D',
  CODE_ANSI_RIGHT = 'C'
};

// Style: WASD
enum {
  CODE_WASD_UP = 'W',
  CODE_WASD_DOWN = 'S',
  CODE_WASD_LEFT = 'A',
  CODE_WASD_RIGHT = 'D'
};

// Style: Vim
enum {
  CODE_VIM_UP = 'K',
  CODE_VIM_DOWN = 'J',
  CODE_VIM_LEFT = 'H',
  CODE_VIM_RIGHT = 'L'
};

enum { CODE_HOTKEY_ACTION_SAVE = 'Z', CODE_HOTKEY_ALTERNATE_ACTION_SAVE = 'P' };

} // namespace Code
} // namespace Keypress

int GetLines(std::string filename) {
  std::ifstream stateFile(filename);
  using iter = std::istreambuf_iterator<char>;
  const auto noOfLines = std::count(iter{stateFile}, iter{}, '\n');
  return noOfLines;
}

std::vector<std::string> get_file_tile_data(std::istream &buf) {
  std::vector<std::string> tempbuffer;
  enum { MAX_WIDTH = 10, MAX_HEIGHT = 10 };
  auto i{0};
  for (std::string tempLine; std::getline(buf, tempLine) && i < MAX_WIDTH;
       i++) {
    std::istringstream temp_filestream(tempLine);
    auto j{0};
    for (std::string a_word;
         std::getline(temp_filestream, a_word, ',') && j < MAX_HEIGHT; j++) {
      tempbuffer.push_back(a_word);
    }
  }
  return tempbuffer;
}

std::vector<Tile> process_file_tile_string_data(std::vector<std::string> buf) {
  std::vector<Tile> result_buf;
  auto tile_processed_counter{0};
  const auto prime_tile_data =
      [&tile_processed_counter](const std::string tile_data) {
        enum FieldIndex { IDX_TILE_VALUE, IDX_TILE_BLOCKED, MAX_NO_TILE_IDXS };
        std::array<int, MAX_NO_TILE_IDXS> tile_internal;
        std::istringstream blocks(tile_data);
        auto idx_id{0};
        for (std::string temptiledata; std::getline(
                 blocks, temptiledata, ':') /*&& idx_id < MAX_NO_TILE_IDXS*/;
             idx_id++) {
          switch (idx_id) {
          case IDX_TILE_VALUE:
            std::get<IDX_TILE_VALUE>(tile_internal) = std::stoi(temptiledata);
            break;
          case IDX_TILE_BLOCKED:
            std::get<IDX_TILE_BLOCKED>(tile_internal) = std::stoi(temptiledata);
            break;
          default:
            std::cout << "ERROR: [tile_processed_counter: "
                      << tile_processed_counter
                      << "]: Read past MAX_NO_TILE_IDXS! (idx no:"
                      << MAX_NO_TILE_IDXS << ")\n";
          }
        }
        tile_processed_counter++;
        return Tile(std::get<IDX_TILE_VALUE>(tile_internal),
                    std::get<IDX_TILE_BLOCKED>(tile_internal));
      };
  std::transform(std::begin(buf), std::end(buf), std::back_inserter(result_buf),
                 prime_tile_data);
  return result_buf;
}

std::tuple<bool, GameBoard>
load_GameBoard_data_from_file(std::string filename) {
  std::ifstream stateFile(filename);
  if (stateFile) {
    const ull savedBoardPlaySize = GetLines(filename);
    const auto file_tile_data = get_file_tile_data(stateFile);
    const auto processed_tile_data =
        process_file_tile_string_data(file_tile_data);
    return std::make_tuple(true,
                           GameBoard(savedBoardPlaySize, processed_tile_data));
  }
  return std::make_tuple(false, GameBoard{});
}

} // namespace

Color::Modifier Tile::tileColor(ull value) {
  std::vector<Color::Modifier> colors{red, yellow, magenta, blue, cyan, yellow,
                                      red, yellow, magenta, blue, green};
  int log = log2(value);
  int index = log < 12 ? log - 1 : 10;

  return colors[index];
}

bool Game::get_and_process_game_stats_string_data(std::istream &stats_file) {
  if (stats_file) {
    for (std::string tempLine; std::getline(stats_file, tempLine);) {
      enum GameStatsFieldIndex {
        IDX_GAME_SCORE_VALUE,
        IDX_GAME_MOVECOUNT,
        MAX_NO_GAME_STATS_IDXS
      };
      std::istringstream line(tempLine);
      auto idx_id{0};
      for (std::string temp; std::getline(line, temp, ':'); idx_id++) {
        switch (idx_id) {
        case IDX_GAME_SCORE_VALUE:
          gamePlayBoard.score = std::stoi(temp);
          break;
        case IDX_GAME_MOVECOUNT:
          gamePlayBoard.moveCount = std::stoi(temp) - 1;
          break;
        default:
          // Error: No fields to process!
          break;
        }
      }
    }
    return true;
  }
  return false;
}

bool Game::load_game_stats_from_file(std::string filename) {
  std::ifstream stats(filename);
  return get_and_process_game_stats_string_data(stats);
}

bool Game::initialiseContinueBoardArray() {
  constexpr auto gameboard_data_filename = "../data/previousGame";
  constexpr auto game_stats_data_filename = "../data/previousGameStats";
  auto loaded_gameboard{false};
  std::tie(loaded_gameboard, gamePlayBoard) =
      load_GameBoard_data_from_file(gameboard_data_filename);

  return (loaded_gameboard &&
          load_game_stats_from_file(game_stats_data_filename));
}

void Game::drawBoard() const {

  clearScreen();
  drawAscii();
  drawScoreBoard(std::cout);
  std::cout << gamePlayBoard;
}

void Game::drawScoreBoard(std::ostream &out_stream) const {
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
  out_stream << outer_border_padding << top_board << "\n";
  out_stream << outer_border_padding << vertical_border_pattern
             << inner_border_padding << bold_on << score_text_label << bold_off
             << std::string(inner_padding_length -
                                std::string{score_text_label}.length() -
                                std::to_string(gamePlayBoard.score).length(),
                            border_padding_char)
             << gamePlayBoard.score << inner_border_padding
             << vertical_border_pattern << "\n";
  if (gamePlayBoard.getPlaySize() == COMPETITION_GAME_BOARD_PLAY_SIZE) {
    const auto tempBestScore =
        (bestScore < gamePlayBoard.score ? gamePlayBoard.score : bestScore);
    out_stream << outer_border_padding << vertical_border_pattern
               << inner_border_padding << bold_on << bestscore_text_label
               << bold_off
               << std::string(inner_padding_length -
                                  std::string{bestscore_text_label}.length() -
                                  std::to_string(tempBestScore).length(),
                              border_padding_char)
               << tempBestScore << inner_border_padding
               << vertical_border_pattern << "\n";
  }
  out_stream << outer_border_padding << vertical_border_pattern
             << inner_border_padding << bold_on << moves_text_label << bold_off
             << std::string(
                    inner_padding_length -
                        std::string{moves_text_label}.length() -
                        std::to_string(gamePlayBoard.MoveCount()).length(),
                    border_padding_char)
             << gamePlayBoard.MoveCount() << inner_border_padding
             << vertical_border_pattern << "\n";
  out_stream << outer_border_padding << bottom_board << "\n \n";
}

void Game::drawInputControls() {
  constexpr auto input_commands_text = u8R"(
  W or K or ↑ => Up
  A or H or ← => Left
  S or J or ↓ => Down
  D or L or → => Right
  Z or P => Save

  Press the keys to start and continue.

)";

  constexpr auto invalid_prompt_text = "Invalid input. Please try again.";
  constexpr auto sp = "  ";
  std::ostringstream str_os;
  std::ostringstream invalid_prompt_richtext;
  invalid_prompt_richtext << red << sp << invalid_prompt_text << def << "\n\n";

  str_os << input_commands_text;

  if (input_err == KeyInputErrorStatus::STATUS_INPUT_ERROR) {
    str_os << invalid_prompt_richtext.str();
    input_err = KeyInputErrorStatus::STATUS_INPUT_VALID;
  }
  std::cout << str_os.str();
}

void Game::input() {

  using namespace Keypress::Code;
  char c;
  getInput(c);

  if (c == CODE_ANSI_TRIGGER_1) {
    getInput(c);
    if (c == CODE_ANSI_TRIGGER_2) {
      getInput(c);
      switch (c) {
      case CODE_ANSI_UP:
        decideMove(UP);
        return;
      case CODE_ANSI_DOWN:
        decideMove(DOWN);
        return;
      case CODE_ANSI_RIGHT:
        decideMove(RIGHT);
        return;
      case CODE_ANSI_LEFT:
        decideMove(LEFT);
        return;
      }
    }
  }

  switch (toupper(c)) {

  case CODE_WASD_UP:
  case CODE_VIM_UP:
    decideMove(UP);
    break;
  case CODE_WASD_LEFT:
  case CODE_VIM_LEFT:
    decideMove(LEFT);
    break;
  case CODE_WASD_DOWN:
  case CODE_VIM_DOWN:
    decideMove(DOWN);
    break;
  case CODE_WASD_RIGHT:
  case CODE_VIM_RIGHT:
    decideMove(RIGHT);
    break;
  case CODE_HOTKEY_ACTION_SAVE:
  case CODE_HOTKEY_ALTERNATE_ACTION_SAVE:
    gamestatus[FLAG_SAVED_GAME] = true;
    break;
  default:
    input_err = KeyInputErrorStatus::STATUS_INPUT_ERROR;
    break;
  }
}

void Game::decideMove(Directions d) {

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

void Game::statistics() const {
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

  std::cout << stats_richtext.str();
}

void Game::saveStats() const {
  Stats stats;
  stats.collectStatistics();
  stats.bestScore = stats.bestScore < gamePlayBoard.score ?
                        gamePlayBoard.score :
                        stats.bestScore;
  stats.gameCount++;
  stats.winCount = gamePlayBoard.hasWon() ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += gamePlayBoard.MoveCount();
  stats.totalDuration += duration;

  std::fstream statistics("../data/statistics.txt");
  statistics << stats.bestScore << std::endl
             << stats.gameCount << std::endl
             << stats.winCount << std::endl
             << stats.totalMoveCount << std::endl
             << stats.totalDuration;

  statistics.close();
}

void Game::saveScore() const {
  Scoreboard s;
  s.score = gamePlayBoard.score;
  s.win = gamePlayBoard.hasWon();
  s.moveCount = gamePlayBoard.MoveCount();
  s.largestTile = gamePlayBoard.largestTile;
  s.duration = duration;
  s.save();
}

void Game::saveState() const {
  std::remove("../data/previousGame");
  std::remove("../data/previousGameStats");
  std::fstream stats("../data/previousGameStats", std::ios_base::app);
  std::fstream stateFile("../data/previousGame", std::ios_base::app);
  stateFile << gamePlayBoard.printState();
  stateFile.close();
  stats << gamePlayBoard.score << ":" << gamePlayBoard.MoveCount();
  stats.close();
}

void Game::drawEndScreen() {
  constexpr auto win_game_text = "You win! Congratulations!";
  constexpr auto lose_game_text = "Game over! You lose.";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream win_richtext;
  win_richtext << green << bold_on << sp << win_game_text << def << bold_off
               << "\n\n\n";

  std::ostringstream lose_richtext;
  lose_richtext << red << bold_on << sp << lose_game_text << def << bold_off
                << "\n\n\n";

  if (gamePlayBoard.hasWon()) {
    str_os << win_richtext.str();
  } else {
    str_os << lose_richtext.str();
  }
  std::cout << str_os.str();
}

void Game::drawGameState() {
  constexpr auto state_saved_text =
      "The game has been saved. Feel free to take a break.";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream state_saved_richtext;
  state_saved_richtext << green << bold_on << sp << state_saved_text << def
                       << bold_off << "\n\n";

  if (gamestatus[FLAG_SAVED_GAME]) {
    str_os << state_saved_richtext.str();
    gamestatus[FLAG_SAVED_GAME] = false;
  }
  std::cout << str_os.str();
}

void Game::drawGraphics() {
  drawBoard();
  drawGameState();
  drawInputControls();
}

void Game::process_gamelogic() {
  gamePlayBoard.unblockTiles();
  if (gamePlayBoard.moved) {
    gamePlayBoard.addTile();
    gamePlayBoard.registerMoveByOne();
  }

  if (gamePlayBoard.hasWon()) {
    gamestatus[FLAG_WIN] = true;
  }
  if (!gamePlayBoard.canMove()) {
    gamestatus[FLAG_END_GAME] = true;
  }
}

bool Game::process_gameStatus() {
  if (gamestatus[FLAG_WIN]) {
    // break if question asked
    return false;
  }
  if (gamestatus[FLAG_END_GAME]) {
    // End endless_mode;
    return false;
  }
  if (gamestatus[FLAG_SAVED_GAME]) {
    saveState();
  }
  return true;
}

bool Game::soloGameLoop() {
  process_gamelogic();

  drawGraphics();
  if (!gamestatus[FLAG_END_GAME]) {
    // Fast track: Game has ended, no input can be done.
    input();
  }
  const auto loop_again = process_gameStatus();
  return loop_again;
}

void Game::endlessGameLoop() {
  while (soloGameLoop())
    ;
}

void Game::playGame(ContinueStatus cont) {
  auto startTime = std::chrono::high_resolution_clock::now();
  endlessGameLoop();
  auto finishTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finishTime - startTime;
  duration = elapsed.count();

  drawBoard();
  drawEndScreen();

  if (gamePlayBoard.getPlaySize() == COMPETITION_GAME_BOARD_PLAY_SIZE &&
      cont == ContinueStatus::STATUS_END_GAME) {
    statistics();
    saveStats();
    newline(2);
    saveScore();
  }
}

ull Game::setBoardSize() {
  const auto invalid_prompt_text = {
      "Invalid input. Gameboard size should range from ", " to ", "."};
  //  constexpr auto num_of_invalid_prompt_text = 3;
  constexpr auto no_save_found_text =
      "No saved game found. Starting a new game.";
  constexpr auto board_size_prompt_text =
      "Enter gameboard size (NOTE: Scores and statistics will be saved only for the 4x4 gameboard): ";
  constexpr auto sp = "  ";

  enum { MIN_GAME_BOARD_PLAY_SIZE = 3, MAX_GAME_BOARD_PLAY_SIZE = 10 };

  std::ostringstream str_os;
  std::ostringstream error_prompt_richtext;
  error_prompt_richtext << red << sp << std::begin(invalid_prompt_text)[0]
                        << MIN_GAME_BOARD_PLAY_SIZE
                        << std::begin(invalid_prompt_text)[1]
                        << MAX_GAME_BOARD_PLAY_SIZE
                        << std::begin(invalid_prompt_text)[2] << def << "\n\n";
  std::ostringstream no_save_richtext;
  no_save_richtext << red << bold_on << sp << no_save_found_text << def
                   << bold_off << "\n\n";
  std::ostringstream board_size_prompt_richtext;
  board_size_prompt_richtext << bold_on << sp << board_size_prompt_text
                             << bold_off;

  bool err = false;
  ull userInput_PlaySize{0};

  while ((userInput_PlaySize < MIN_GAME_BOARD_PLAY_SIZE ||
          userInput_PlaySize > MAX_GAME_BOARD_PLAY_SIZE)) {
    clearScreen();
    drawAscii();

    if (err) {
      str_os << error_prompt_richtext.str();
    } else if (noSave) {
      str_os << no_save_richtext.str();
      noSave = false;
    }

    str_os << board_size_prompt_richtext.str();
    std::cout << str_os.str();

    std::cin >> userInput_PlaySize;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::int32_t>::max(), '\n');
    err = true;
  }
  return userInput_PlaySize;
}

void Game::startGame() {

  Stats stats;
  if (stats.collectStatistics()) {
    bestScore = stats.bestScore;
  }

  ull userInput_PlaySize = setBoardSize();

  gamePlayBoard = GameBoard(userInput_PlaySize);
  gamePlayBoard.addTile();

  playGame(ContinueStatus::STATUS_END_GAME);
}

void Game::continueGame() {

  Stats stats;
  if (stats.collectStatistics()) {
    bestScore = stats.bestScore;
  }

  if (initialiseContinueBoardArray()) {
    playGame(ContinueStatus::STATUS_CONTINUE);
  } else {
    noSave = true;
    startGame();
  }
}
