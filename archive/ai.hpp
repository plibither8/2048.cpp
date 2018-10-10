#pragma once

#include "global.hpppp"
#include "scores.hpppp"
#include "statistics.hpppp"
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <limits>
#include <math.hpp>
#include <string>
#include <vector>

enum Directions { UP, DOWN, RIGHT, LEFT };

class Tile {

public:
  Tile() : value(0), blocked(false) {}
  ull value;
  bool blocked;
  Color::Modifier tileColor(ull);
};

Color::Modifier Tile::tileColor(ull value) {
  std::vector<Color::Modifier> colors{red, yellow, magenta, blue, cyan, yellow,
                                      red, yellow, magenta, blue, green};
  int log = log2(value);
  int index = log < 12 ? log - 1 : 10;

  return colors[index];
}

class Game {

private:
  bool moved;
  bool win;
  bool boardFull;
  bool rexit;
  ull score;
  ull bestScore;
  ull largestTile;
  long long moveCount;
  double duration;
  ull BOARD_SIZE;
  std::vector<std::vector<Tile>> board;

  void initialiseBoardArray();
  bool addTile();
  void collectFreeTiles(std::vector<std::vector<int>> &freeTiles);
  void drawBoard();
  void input(int err = 0);
  bool canMove();
  bool testAdd(int, int, ull);
  void unblockTiles();
  void decideMove(Directions);
  void move(int, int, int, int);
  void statistics();
  void saveStats();
  void saveScore();

public:
  Game()
      : win(false), moved(true), boardFull(false), rexit(false), score(0),
        moveCount(-2), largestTile(2) {}
  void startGame(int err = 0);
};

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

bool Game::addTile() {

  std::vector<std::vector<int>> freeTiles;
  collectFreeTiles(freeTiles);

  if (!freeTiles.size()) {
    boardFull = true;
  }

  std::vector<int> randomFreeTile = freeTiles.at(rand() % freeTiles.size());
  int x = randomFreeTile.at(1);
  int y = randomFreeTile.at(0);
  board[y][x].value = rand() % 100 > 89 ? 4 : 2;

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
  std::cout << "  +---------------------------+";
  endl();
  std::cout << "  | " << bold_on << "SCORE:" << bold_off << std::setw(19)
            << score << " |";
  endl();
  if (BOARD_SIZE == 4) {
    std::cout << "  | " << bold_on << "BEST SCORE:" << bold_off << std::setw(14)
              << (bestScore < score ? score : bestScore) << " |";
    endl();
  }
  std::cout << "  | " << bold_on << "MOVES:" << bold_off << std::setw(19)
            << moveCount << " |";
  endl();
  std::cout << "  +---------------------------+";
  endl(2);

  for (int y = 0; y < BOARD_SIZE; y++) {

    std::cout << "  +";
    for (int i = 0; i < BOARD_SIZE; i++) {
      std::cout << "------+";
    }
    endl();
    std::cout << " ";

    for (int x = 0; x < BOARD_SIZE; x++) {

      Tile currentTile = board[y][x];

      std::cout << " | ";
      if (!currentTile.value) {
        std::cout << "    ";
      } else {
        std::cout << currentTile.tileColor(currentTile.value) << bold_on
                  << std::setw(4) << currentTile.value << bold_off << def;
      }
    }

    std::cout << " | ";
    endl();
  }

  std::cout << "  +";
  for (int i = 0; i < BOARD_SIZE; i++) {
    std::cout << "------+";
  }
  endl(3);
}

void Game::input(int err) {

  moved = false;
  char c;

  std::cout << "  W => Up";
  endl();
  std::cout << "  A => Left";
  endl();
  std::cout << "  S => Down";
  endl();
  std::cout << "  D => Right";
  endl(2);
  std::cout << "  Press the keys to start and continue.";
  endl();

  if (err) {
    std::cout << red << "  Invalid input. Please try again." << def;
    endl(2);
  }

  c = getch();

  endl(4);

  switch (toupper(c)) {

  case 'W':
    decideMove(UP);
    break;
  case 'A':
    decideMove(LEFT);
    break;
  case 'S':
    decideMove(DOWN);
    break;
  case 'D':
    decideMove(RIGHT);
    break;
  default:
    drawBoard();
    input(1);
    break;
  }

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
  std::cout << yellow << "  ----------" << def;
  endl();
  std::cout << "  Final score:       " << bold_on << score << bold_off;
  endl();
  std::cout << "  Largest Tile:      " << bold_on << largestTile << bold_off;
  endl();
  std::cout << "  Number of moves:   " << bold_on << moveCount << bold_off;
  endl();
  std::cout << "  Time taken:        " << bold_on << duration << " seconds"
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

void Game::startGame(int err) {

  Stats stats;
  stats.collectStatistics();
  bestScore = stats.bestScore;

  clearScreen();
  drawAscii();

  if (err) {
    std::cout << red
              << "  Invalid input. Gameboard size should range from 3 to 6."
              << def;
    endl(2);
  }

  std::cout
      << bold_on
      << "  Enter gameboard size (NOTE: Scores and statistics will be saved only for the 4x4 gameboard): "
      << bold_off;

  if (!(std::cin >> BOARD_SIZE) || BOARD_SIZE < 3 || BOARD_SIZE > 10) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::int32_t>::max(), '\n');
    startGame(1);
  }

  initialiseBoardArray();

  auto startTime = std::chrono::high_resolution_clock::now();

  addTile();

  while (1) {

    if (moved) {
      if (!addTile()) {
        drawBoard();
        break;
      }
    }

    drawBoard();
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

  if (BOARD_SIZE == 4) {
    statistics();
    saveStats();
    endl(2);
    saveScore();
  }
}
