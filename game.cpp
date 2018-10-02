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
  for (int i = 0; i < BOARD_SIZE; i++) {
    std::vector<Tile> bufferArray;
    for (int j = 0; j < BOARD_SIZE; j++) {
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
    BOARD_SIZE = GetLines();
    initialiseBoardArray();
    std::string tempArr[BOARD_SIZE][BOARD_SIZE];
    int i = 0, j, k;
    while (std::getline(stateFile, tempLine, '\n') && i < BOARD_SIZE) {
      std::stringstream line(tempLine);
      j = 0;
      while (std::getline(line, temp, ',') && j < BOARD_SIZE) {
        tempArr[i][j] = temp;
        j++;
      }
      i++;
    }

    for (int i = 0; i < BOARD_SIZE; i++) {
      std::vector<Tile> bufferArray;
      for (int j = 0; j < BOARD_SIZE; j++) {
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

  std::vector<std::vector<int>> freeTiles;
  collectFreeTiles(freeTiles);

  if (!freeTiles.size()) {
    boardFull = true;
  }

  std::vector<int> randomFreeTile = freeTiles.at(randInt() % freeTiles.size());
  int x = randomFreeTile.at(1);
  int y = randomFreeTile.at(0);
  board[y][x].value = randInt() % 100 > 89 ? 4 : 2;

  moveCount++;
  moved = true;

  if (rexit) {
    return !rexit;
  }

  return canMove();
}

void Game::collectFreeTiles(std::vector<std::vector<int>> &freeTiles) {

  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
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
  std::cout << "  ┌───────────────────────────┐";
  endl();
  std::cout << "  │ " << bold_on << "SCORE:" << bold_off << std::setw(19)
            << score << " │";
  endl();
  if (BOARD_SIZE == 4) {
    std::cout << "  │ " << bold_on << "BEST SCORE:" << bold_off << std::setw(14)
              << (bestScore < score ? score : bestScore) << " │";
    endl();
  }
  std::cout << "  │ " << bold_on << "MOVES:" << bold_off << std::setw(19)
            << moveCount << " │";
  endl();
  std::cout << "  └───────────────────────────┘";
  endl(2);

  for (int y = 0; y < BOARD_SIZE; y++) {

    std::cout << "  ";

    if (y == 0) {
      std::cout << "┌";
    } else {
      std::cout << "├";
    }
    for (int i = 0; i < BOARD_SIZE; i++) {
      std::cout << "──────";
      if (i < BOARD_SIZE - 1) {
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
    endl();
    std::cout << " ";

    for (int x = 0; x < BOARD_SIZE; x++) {

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
    endl();
  }

  std::cout << "  └";
  for (int i = 0; i < BOARD_SIZE; i++) {
    std::cout << "──────";
    if (i < BOARD_SIZE - 1) {
      std::cout << "┴";
    } else {
      std::cout << "┘";
    }
  }
  endl(3);
}

void Game::input(int err) {

  using namespace Keypress::Code;
  moved = false;
  char c;

  std::cout << "  W or K => Up";
  endl();
  std::cout << "  A or H => Left";
  endl();
  std::cout << "  S or J => Down";
  endl();
  std::cout << "  D or L => Right";
  endl();
  std::cout << "  Z or P => Save";
  endl(2);
  std::cout << "  Press the keys to start and continue.";
  endl();

  if (err) {
    std::cout << red << "  Invalid input. Please try again." << def;
    endl(2);
  }

  getInput(c);

  if (c == CODE_ANSI_TRIGGER_1) {
    getInput(c);
    if (c == CODE_ANSI_TRIGGER_2) {
      getInput(c);
      endl(4);
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
      endl(4);
    }
  }

  endl(4);

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
    input(1);
    break;
  }

next:
  unblockTiles();
}

bool Game::canMove() {

  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
      if (!board[y][x].value) {
        return true;
      }
    }
  }

  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
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

  if (y < 0 || y > BOARD_SIZE - 1 || x < 0 || x > BOARD_SIZE - 1) {
    return false;
  }

  return board[y][x].value == value;
}

void Game::unblockTiles() {

  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
      board[y][x].blocked = false;
    }
  }
}

void Game::decideMove(Directions d) {

  switch (d) {

  case UP:

    for (int x = 0; x < BOARD_SIZE; x++) {
      int y = 1;
      while (y < BOARD_SIZE) {
        if (board[y][x].value) {
          move(y, x, -1, 0);
        }
        y++;
      }
    }
    break;

  case DOWN:

    for (int x = 0; x < BOARD_SIZE; x++) {
      int y = BOARD_SIZE - 2;
      while (y >= 0) {
        if (board[y][x].value) {
          move(y, x, 1, 0);
        }
        y--;
      }
    }
    break;

  case LEFT:

    for (int y = 0; y < BOARD_SIZE; y++) {
      int x = 1;
      while (x < BOARD_SIZE) {
        if (board[y][x].value) {
          move(y, x, 0, -1);
        }
        x++;
      }
    }
    break;

  case RIGHT:

    for (int y = 0; y < BOARD_SIZE; y++) {
      int x = BOARD_SIZE - 2;
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
      if (targetTile.value == 2048) {
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

  if (k + l == 1 && (k == 1 ? y : x) < BOARD_SIZE - 2) {
    move(y + k, x + l, k, l);
  } else if (k + l == -1 && (k == -1 ? y : x) > 1) {
    move(y + k, x + l, k, l);
  }
}

void Game::statistics() {

  std::cout << yellow << "  STATISTICS" << def;
  endl();
  std::cout << yellow << "  ──────────" << def;
  endl();
  std::cout << "  Final score:       " << bold_on << score << bold_off;
  endl();
  std::cout << "  Largest Tile:      " << bold_on << largestTile << bold_off;
  endl();
  std::cout << "  Number of moves:   " << bold_on << moveCount << bold_off;
  endl();
  std::cout << "  Time taken:        " << bold_on << secondsFormat(duration)
            << bold_off;
  endl();
}

void Game::saveStats() {
  Stats stats;
  stats.collectStatistics();
  stats.bestScore = stats.bestScore < score ? score : stats.bestScore;
  stats.gameCount++;
  stats.winCount = win ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += moveCount;
  stats.totalDuration += duration;

  std::fstream statistics("./data/statistics.txt");
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
  return;
}

void Game::saveState() {
  std::remove("./data/previousGame");
  std::remove("./data/previousGameStats");
  std::fstream stats("./data/previousGameStats", std::ios_base::app);
  std::fstream stateFile("./data/previousGame", std::ios_base::app);
  for (int y = 0; y < BOARD_SIZE; y++) {
    for (int x = 0; x < BOARD_SIZE; x++) {
      stateFile << board[y][x].value << ":" << board[y][x].blocked << ",";
      endl();
    }
    stateFile << "\n";
  }
  stateFile.close();
  stats << score << ":" << moveCount;
  stats.close();
}

void Game::playGame(int cont) {

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
      endl(2);
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
    endl(3);
  } else {
    std::cout << red << bold_on << msg << def << bold_off;
    endl(3);
  }

  if (BOARD_SIZE == 4 && !cont) {
    statistics();
    saveStats();
    endl(2);
    saveScore();
  }
}

void Game::setBoardSize() {

  bool err = false;
  BOARD_SIZE = 0;
  while ((BOARD_SIZE < 3 || BOARD_SIZE > 10)) {
    std::cout << "InSide While";
    clearScreen();
    drawAscii();

    if (err) {
      std::cout << red
                << "  Invalid input. Gameboard size should range from 3 to 6."
                << def;
      endl(2);
    } else if (noSave) {
      std::cout << red << bold_on << "No save game exist, Starting a new game."
                << def << bold_off;
      endl(2);
      noSave = false;
    }

    std::cout << bold_on
              << "  Enter gameboard size (NOTE: Scores and statistics will be "
                 "saved only for the 4x4 gameboard): "
              << bold_off;

    std::cin >> BOARD_SIZE;
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

  playGame(0);
}

void Game::continueGame() {

  Stats stats;
  if (stats.collectStatistics()) {
    bestScore = stats.bestScore;
  }

  clearScreen();
  drawAscii();

  initialiseContinueBoardArray();

  playGame(1);
}
