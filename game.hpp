#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include <math.h>
#include "global.hpp"
#include "scores.hpp"

enum Directions {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

class Tile {

    public:
        Tile(): value(0), blocked(false) {}
        ull value;
        bool blocked;
        Color::Modifier tileColor(ull);

};

Color::Modifier Tile::tileColor(ull value) {
    std::vector<Color::Modifier> colors{
        red,
        yellow,
        magenta,
        blue,
        cyan,
        yellow,
        red,
        yellow,
        magenta,
        blue,
        green
    };
    int log = log2(value);
    int index = log < 12 ? log - 1 : 10;

    return colors[index];
}

class Game {

    private:
        bool moved;
        bool win;
        bool boardFull;
        bool exit;
        ull score;
        ull largestTile;
        long long moveCount;
        double duration;

        bool addTile();
        void collectFreeTiles(std::vector<std::vector<int> > &freeTiles);
        void padding(ull);
        void drawBoard();
        void input(int err = 0);
        bool canMove();
        bool testAdd(int, int, ull);
        void unblockTiles();
        void decideMove(Directions);
        void move(int, int, int, int);
        void statistics();
        void saveScorePrompt();
        void saveScore();

    public:

        Game() : win(false), moved(true), boardFull(false), exit(false), score(0), moveCount(-2), largestTile(2) {}

        void startGame();

        std::vector<std::vector <Tile> > board{4, std::vector<Tile>(4)};

};

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

    if (exit) {
        return !exit;
    }

    return canMove();

}

void Game::collectFreeTiles(std::vector<std::vector<int> > &freeTiles) {

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!board[y][x].value) {
                std::vector<int> newEmpty {y, x};
                freeTiles.push_back(newEmpty);
            }
        }
    }

}

void Game::padding(ull value) {
    int length = 0;
    while (value) {
        value /= 10;
        length++;
    }
    while (4 - length++) {
        std::cout << " ";
    }
}

void Game::drawBoard() {

    clearScreen();

    drawAscii();
    std::cout << "  +---------------------------+"; endl();
    std::cout << "  | SCORE:" << std::setw(19) << score << " |"; endl();
    std::cout << "  | MOVES:" << std::setw(19) << moveCount << " |"; endl();
    std::cout << "  +---------------------------+"; endl(2);

    for (int y = 0; y < 4; y++) {

        std::cout << "  +------+------+------+------+" ; endl();
        std::cout << " ";

        for (int x = 0; x < 4; x++) {

            Tile currentTile = board[y][x];

            std::cout << " | ";
            if (!currentTile.value) {
                std::cout << std::setw(7);
            }
            else {
                padding(currentTile.value);
                std::cout << currentTile.tileColor(currentTile.value) << bold_on << currentTile.value << bold_off << def;
            }

        }

        std::cout << " | "; endl();

    }

    std::cout << "  +------+------+------+------+"; endl(3);

}

void Game::input(int err) {

    moved = false;
    char c;

    std::cout << "  W => Up"; endl();
    std::cout << "  A => Left"; endl();
    std::cout << "  S => Down"; endl();
    std::cout << "  D => Right"; endl(2);
    std::cout << "  Press the keys to start and continue."; endl();

    if (err) {
        std::cout << red << "  Invalid input. Please try again." << def; endl(2);
    }

    c = getch();
    
    endl(4);

    switch(toupper(c)) {

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

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!board[y][x].value) {
                return true;
            }
        }
    }

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
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

    if (y < 0 || y > 3 || x < 0 || x > 3) {
        return false;
    }

    return board[y][x].value == value;

}

void Game::unblockTiles() {

    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            board[y][x].blocked = false;
        }
    }

}

void Game::decideMove(Directions d) {

    switch (d) {

        case UP:

            for (int x = 0; x < 4; x++) {
                int y = 1;
                while (y < 4) {
                    if (board[y][x].value) {
                        move(y, x, -1, 0);
                    }
                    y++;
                }
            }
            break;

        case DOWN:

            for (int x = 0; x < 4; x++) {
                int y = 2;
                while (y >= 0) {
                    if (board[y][x].value) {
                        move(y, x, 1, 0);
                    }
                    y--;
                }
            }
            break;

        case LEFT:

            for (int y = 0; y < 4; y++) {
                int x = 1;
                while (x < 4) {
                    if (board[y][x].value) {
                        move(y, x, 0, -1);
                    }
                    x++;
                }
            }
            break;

        case RIGHT:

            for (int y = 0; y < 4; y++) {
                int x = 2;
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

        largestTile = largestTile < targetTile.value ? targetTile.value : largestTile;
        if (!win) {
            if (targetTile.value == 2048) {
                win = true;
                std::cout << green << bold_on << "  You win! Press any key to continue or 'x' to exit: " << bold_off << def;
                char c;
                std::cin >> c;
                switch (toupper(c)) {
                    case 'X':
                        exit = true;
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
    
    if (k + l == 1 && (k == 1 ? y : x) < 2) {
        move(y + k, x + l, k, l);
    }
    else if (k + l == -1 && (k == -1 ? y : x) > 1) {
        move(y + k, x + l, k, l);
    }

}

void Game::statistics() {

    std::cout << yellow << "  STATISTICS" << def; endl();
    std::cout << yellow << "  ----------" << def; endl();
    std::cout << "  Final score:       " << bold_on << score << bold_off; endl();
    std::cout << "  Largest Tile:      " << bold_on << largestTile << bold_off; endl();
    std::cout << "  Number of moves:   " << bold_on << moveCount << bold_off; endl();
    std::cout << "  Time taken:        " << bold_on << duration << " seconds" << bold_off; endl();

}

void Game::saveScorePrompt() {

    std::cout << green << bold_on << "  Do you want to save this score?" << bold_off << def; endl();
    std::cout << green << "  Press 'y' for YES or any other key to exit: " << def;
    char c;
    std::cin >> c;

    switch(toupper(c)) {
        case 'Y':
            endl();
            saveScore();
            break;
        default:
            endl();
            std::cout << yellow << "  Goodbye!" << def; endl();
            break;
    }

}

void Game::saveScore() {
    Score s;
    s.score = score;
    s.save();
    return;
}

void Game::startGame() {

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
        std::cout << green << bold_on << msg << def << bold_off; endl(3);
    }
    else {
        std::cout << red << bold_on << msg << def << bold_off; endl(3);
    }

    statistics();
    endl(2);
    saveScorePrompt();

}