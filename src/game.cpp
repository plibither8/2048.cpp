#include "game.hpp"

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
} // namespace

Color::Modifier Tile::tileColor(ull value) {
  std::vector<Color::Modifier> colors{red, yellow, magenta, blue, cyan, yellow,
                                      red, yellow, magenta, blue, green};
  int log = log2(value);
  int index = log < 12 ? log - 1 : 10;

  return colors[index];
}

void Game::initialiseBoardArray() {
  for (int i = 0; i < gameBoardPlaySize; i++) {
    std::vector<Tile> bufferArray;
    for (int j = 0; j < gameBoardPlaySize; j++) {
      Tile bufferTile;
      bufferArray.push_back(bufferTile);
    }
    board.push_back(bufferArray);
  }
}

int GetLines() {
  int noOfLines = 0;
  std::string tempLine;
  std::ifstream stateFile("./data/previousGame");
  while (std::getline(stateFile, tempLine, '\n')) {
    noOfLines++;
  }
  stateFile.close();
  return noOfLines;
}

void Game::initialiseContinueBoardArray() {

  std::ifstream stateFile("./data/previousGame");
  if ((bool)stateFile) {
    std::string temp, tempLine, tempBlock;
    gameBoardPlaySize = GetLines();
    initialiseBoardArray();
    std::string tempArr[gameBoardPlaySize][gameBoardPlaySize];
    int i = 0, j, k;
    while (std::getline(stateFile, tempLine, '\n') && i < gameBoardPlaySize) {
      std::stringstream line(tempLine);
      j = 0;
      while (std::getline(line, temp, ',') && j < gameBoardPlaySize) {
        tempArr[i][j] = temp;
        j++;
      }
      i++;
    }

    for (int i = 0; i < gameBoardPlaySize; i++) {
      std::vector<Tile> bufferArray;
      for (int j = 0; j < gameBoardPlaySize; j++) {
        std::stringstream blocks(tempArr[i][j]);
        k = 0;
        Tile bufferTile;
        while (std::getline(blocks, tempBlock, ':')) {
          if (k == 0) {
            board[i][j].value = std::stoi(tempBlock);
          } else if (k == 1) {
            board[i][j].blocked = std::stoi(tempBlock);
          }
          k++;
        }
      }
    }
    stateFile.close();
    std::ifstream stats("./data/previousGameStats");
    while (std::getline(stats, tempLine, '\n')) {
      std::stringstream line(tempLine);
      k = 0;
      while (std::getline(line, temp, ':')) {
        if (k == 0)
          score = std::stoi(temp);
        else if (k == 1)
          moveCount = std::stoi(temp) - 1;
        k++;
      }
    }

  } else {
    noSave = true;
    startGame();
  }
}

bool Game::addTile() {

  constexpr auto CHANCE_OF_VALUE_FOUR_OVER_TWO = 89; // Percentage
  std::vector<std::vector<int>> freeTiles;
  collectFreeTiles(freeTiles);

  if (!freeTiles.size()) {
    boardFull = true;
  }

  std::vector<int> randomFreeTile = freeTiles.at(randInt() % freeTiles.size());
  int x = randomFreeTile.at(1);
  int y = randomFreeTile.at(0);
  board[y][x].value = randInt() % 100 > CHANCE_OF_VALUE_FOUR_OVER_TWO ? 4 : 2;

  moveCount++;
  moved = true;

  if (rexit) {
    return !rexit;
  }

  return canMove();
}

void Game::collectFreeTiles(std::vector<std::vector<int>> &freeTiles) {

  for (int y = 0; y < gameBoardPlaySize; y++) {
    for (int x = 0; x < gameBoardPlaySize; x++) {
      if (!board[y][x].value) {
        std::vector<int> newEmpty{y, x};
        freeTiles.push_back(newEmpty);
      }
    }
  }
}

void Game::drawBoard() {

  clearScreen();
  drawAscii();
  drawScoreBoard(std::cout);

  for (int y = 0; y < gameBoardPlaySize; y++) {

    std::cout << "  ";

    if (y == 0) {
      std::cout << "┌";
    } else {
      std::cout << "├";
    }
    for (int i = 0; i < gameBoardPlaySize; i++) {
      std::cout << "──────";
      if (i < gameBoardPlaySize - 1) {
        if (y == 0) {
          std::cout << "┬";
        } else {
          std::cout << "┼";
        }
      } else {
        if (y == 0) {
          std::cout << "┐";
        } else {
          std::cout << "┤";
        }
      }
    }
    newline();
    std::cout << " ";

    for (int x = 0; x < gameBoardPlaySize; x++) {

      Tile currentTile = board[y][x];

      std::cout << " │ ";
      if (!currentTile.value) {
        std::cout << "    ";
      } else {
        std::cout << currentTile.tileColor(currentTile.value) << bold_on
                  << std::setw(4) << currentTile.value << bold_off << def;
      }
    }

    std::cout << " │ ";
    newline();
  }

  std::cout << "  └";
  for (int i = 0; i < gameBoardPlaySize; i++) {
    std::cout << "──────";
    if (i < gameBoardPlaySize - 1) {
      std::cout << "┴";
    } else {
      std::cout << "┘";
    }
  }
  newline(3);
}

void Game::drawScoreBoard(std::ostream &out_stream) {
  constexpr auto score_text_label = "SCORE:";
  constexpr auto bestscore_text_label = "BEST CORE:";
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
                                std::to_string(score).length(),
                            border_padding_char)
             << score << inner_border_padding << vertical_border_pattern
             << "\n";
  if (gameBoardPlaySize == COMPETITION_GAME_BOARD_PLAY_SIZE) {
    const auto tempBestScore = (bestScore < score ? score : bestScore);
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
             << std::string(inner_padding_length -
                                std::string{moves_text_label}.length() -
                                std::to_string(moveCount).length(),
                            border_padding_char)
             << moveCount << inner_border_padding << vertical_border_pattern
             << "\n";
  out_stream << outer_border_padding << bottom_board << "\n \n";
}

void Game::input(KeyInputErrorStatus err) {

  using namespace Keypress::Code;
  moved = false;
  char c;

  std::cout << "  W or K or \u2191 => Up";
  newline();
  std::cout << "  A or H or \u2190 => Left";
  newline();
  std::cout << "  S or J or \u2193 => Down";
  newline();
  std::cout << "  D or L or \u2192 => Right";
  newline();
  std::cout << "  Z or P => Save";
  newline(2);
  std::cout << "  Press the keys to start and continue.";
  newline();

  if (err == KeyInputErrorStatus::STATUS_INPUT_ERROR) {
    std::cout << red << "  Invalid input. Please try again." << def;
    newline(2);
  }

  getInput(c);

  if (c == CODE_ANSI_TRIGGER_1) {
    getInput(c);
    if (c == CODE_ANSI_TRIGGER_2) {
      getInput(c);
      newline(4);
      switch (c) {
      case CODE_ANSI_UP:
        decideMove(UP);
        goto next;
      case CODE_ANSI_DOWN:
        decideMove(DOWN);
        goto next;
      case CODE_ANSI_RIGHT:
        decideMove(RIGHT);
        goto next;
      case CODE_ANSI_LEFT:
        decideMove(LEFT);
        goto next;
      }
    } else {
      newline(4);
    }
  }

  newline(4);

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
    saveState();
    stateSaved = true;
    break;
  default:
    drawBoard();
    input(KeyInputErrorStatus::STATUS_INPUT_ERROR);
    break;
  }

next:
  unblockTiles();
}

bool Game::canMove() {

  for (int y = 0; y < gameBoardPlaySize; y++) {
    for (int x = 0; x < gameBoardPlaySize; x++) {
      if (!board[y][x].value) {
        return true;
      }
    }
  }

  for (int y = 0; y < gameBoardPlaySize; y++) {
    for (int x = 0; x < gameBoardPlaySize; x++) {
      if (testAdd(y + 1, x, board[y][x].value)) {
        return true;
      }
      if (testAdd(y - 1, x, board[y][x].value)) {
        return true;
      }
      if (testAdd(y, x + 1, board[y][x].value)) {
        return true;
      }
      if (testAdd(y, x - 1, board[y][x].value)) {
        return true;
      }
    }
  }

  return false;
}

bool Game::testAdd(int y, int x, ull value) {

  if (y < 0 || y > gameBoardPlaySize - 1 || x < 0 ||
      x > gameBoardPlaySize - 1) {
    return false;
  }

  return board[y][x].value == value;
}

void Game::unblockTiles() {

  for (int y = 0; y < gameBoardPlaySize; y++) {
    for (int x = 0; x < gameBoardPlaySize; x++) {
      board[y][x].blocked = false;
    }
  }
}

void Game::decideMove(Directions d) {

  switch (d) {

  case UP:

    for (int x = 0; x < gameBoardPlaySize; x++) {
      int y = 1;
      while (y < gameBoardPlaySize) {
        if (board[y][x].value) {
          move(y, x, -1, 0);
        }
        y++;
      }
    }
    break;

  case DOWN:

    for (int x = 0; x < gameBoardPlaySize; x++) {
      int y = gameBoardPlaySize - 2;
      while (y >= 0) {
        if (board[y][x].value) {
          move(y, x, 1, 0);
        }
        y--;
      }
    }
    break;

  case LEFT:

    for (int y = 0; y < gameBoardPlaySize; y++) {
      int x = 1;
      while (x < gameBoardPlaySize) {
        if (board[y][x].value) {
          move(y, x, 0, -1);
        }
        x++;
      }
    }
    break;

  case RIGHT:

    for (int y = 0; y < gameBoardPlaySize; y++) {
      int x = gameBoardPlaySize - 2;
      while (x >= 0) {
        if (board[y][x].value) {
          move(y, x, 0, 1);
        }
        x--;
      }
    }
    break;
  }
}

void Game::move(int y, int x, int k, int l) {

  constexpr auto GAME_TILE_WINNING_SCORE = 2048;
  Tile &currentTile = board[y][x];
  Tile &targetTile = board[y + k][x + l];

  int A = currentTile.value;
  int B = targetTile.value;
  int C = currentTile.blocked;
  int D = targetTile.blocked;

  if (B && A == B && !C && !D) {

    currentTile.value = 0;
    targetTile.value *= 2;
    score += targetTile.value;
    targetTile.blocked = true;

    largestTile =
        largestTile < targetTile.value ? targetTile.value : largestTile;
    if (!win) {
      if (targetTile.value == GAME_TILE_WINNING_SCORE) {
        win = true;
        std::cout << green << bold_on
                  << "  You win! Press any key to continue or 'x' to exit: "
                  << bold_off << def;
        char c;
        std::cin >> c;
        switch (toupper(c)) {
        case 'X':
          rexit = true;
          break;
        default:
          break;
        }
      }
    }

    moved = true;

  }

  else if (A && !B) {

    targetTile.value = currentTile.value;
    currentTile.value = 0;

    moved = true;
  }

  if (k + l == 1 && (k == 1 ? y : x) < gameBoardPlaySize - 2) {
    move(y + k, x + l, k, l);
  } else if (k + l == -1 && (k == -1 ? y : x) > 1) {
    move(y + k, x + l, k, l);
  }
}

void Game::statistics() {

  std::cout << yellow << "  STATISTICS" << def;
  newline();
  std::cout << yellow << "  ──────────" << def;
  newline();
  std::cout << "  Final score:       " << bold_on << score << bold_off;
  newline();
  std::cout << "  Largest Tile:      " << bold_on << largestTile << bold_off;
  newline();
  std::cout << "  Number of moves:   " << bold_on << moveCount << bold_off;
  newline();
  std::cout << "  Time taken:        " << bold_on << secondsFormat(duration)
            << bold_off;
  newline();
}

void Game::saveStats() {
  Stats stats;
  stats.collectStatistics();
  stats.bestScore = stats.bestScore < score ? score : stats.bestScore;
  stats.gameCount++;
  stats.winCount = win ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += moveCount;
  stats.totalDuration += duration;

  std::fstream statistics("../data/statistics.txt");
  statistics << stats.bestScore << std::endl
             << stats.gameCount << std::endl
             << stats.winCount << std::endl
             << stats.totalMoveCount << std::endl
             << stats.totalDuration;

  statistics.close();
}

void Game::saveScore() {
  Scoreboard s;
  s.score = score;
  s.win = win;
  s.moveCount = moveCount;
  s.largestTile = largestTile;
  s.duration = duration;
  s.save();
}

void Game::saveState() {
  std::remove("../data/previousGame");
  std::remove("../data/previousGameStats");
  std::fstream stats("../data/previousGameStats", std::ios_base::app);
  std::fstream stateFile("../data/previousGame", std::ios_base::app);
  for (int y = 0; y < gameBoardPlaySize; y++) {
    for (int x = 0; x < gameBoardPlaySize; x++) {
      stateFile << board[y][x].value << ":" << board[y][x].blocked << ",";
      newline();
    }
    stateFile << "\n";
  }
  stateFile.close();
  stats << score << ":" << moveCount;
  stats.close();
}

void Game::playGame(ContinueStatus cont) {

  auto startTime = std::chrono::high_resolution_clock::now();

  while (true) {

    if (moved) {
      if (!addTile()) {
        drawBoard();
        break;
      }
    }

    drawBoard();
    if (stateSaved) {
      std::cout << green << bold_on
                << "The game has been saved feel free to take a break." << def
                << bold_off;
      newline(2);
      stateSaved = false;
    }
    input();
  }

  auto finishTime = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> elapsed = finishTime - startTime;
  duration = elapsed.count();

  std::string msg = win ? "  You win!" : "  Game over! You lose.";

  if (win) {
    std::cout << green << bold_on << msg << def << bold_off;
    newline(3);
  } else {
    std::cout << red << bold_on << msg << def << bold_off;
    newline(3);
  }

  if (gameBoardPlaySize == COMPETITION_GAME_BOARD_PLAY_SIZE &&
      cont == ContinueStatus::STATUS_END_GAME) {
    statistics();
    saveStats();
    newline(2);
    saveScore();
  }
}

void Game::setBoardSize() {

  enum { MIN_GAME_BOARD_PLAY_SIZE = 3, MAX_GAME_BOARD_PLAY_SIZE = 10 };
  bool err = false;
  gameBoardPlaySize = 0;
  while ((gameBoardPlaySize < MIN_GAME_BOARD_PLAY_SIZE ||
          gameBoardPlaySize > MAX_GAME_BOARD_PLAY_SIZE)) {
    clearScreen();
    drawAscii();

    if (err) {
      std::cout << red << "  Invalid input. Gameboard size should range from "
                << MIN_GAME_BOARD_PLAY_SIZE << " to "
                << MAX_GAME_BOARD_PLAY_SIZE << "." << def;
      newline(2);
    } else if (noSave) {
      std::cout << red << bold_on << "No save game exist, Starting a new game."
                << def << bold_off;
      newline(2);
      noSave = false;
    }

    std::cout << bold_on
              << "  Enter gameboard size (NOTE: Scores and statistics will be "
                 "saved only for the 4x4 gameboard): "
              << bold_off;

    std::cin >> gameBoardPlaySize;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::int32_t>::max(), '\n');
    err = true;
  }
}

void Game::startGame() {

  Stats stats;
  if (stats.collectStatistics()) {
    bestScore = stats.bestScore;
  }

  setBoardSize();

  initialiseBoardArray();
  addTile();

  playGame(ContinueStatus::STATUS_END_GAME);
}

void Game::continueGame() {

  Stats stats;
  if (stats.collectStatistics()) {
    bestScore = stats.bestScore;
  }

  clearScreen();
  drawAscii();

  initialiseContinueBoardArray();

  playGame(ContinueStatus::STATUS_CONTINUE);
}
