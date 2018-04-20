#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>
#include "global.hpp"

typedef unsigned long long ull;
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

};

class Game {

    private:
        bool moved;
        bool win;
        bool boardFull;
        ull score;
        ull largestTile;
        long long moveCount;
        double duration;

        bool addTile();
        void collectFreeTiles(std::vector<std::vector<int> > &freeTiles);
        void drawBoard();
        void input(int err = 0);
        bool canMove();
        bool testAdd(int y, int x, ull value);
        void unblockTiles();
        void move(Directions d);
        void moveVertical(int y, int x, int k);
        void moveHorizontal(int y, int x, int k);
        void statistics();

    public:

        Game() : win(false), moved(true), boardFull(false), score(0), moveCount(-2), largestTile(2) {}

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

    return canMove();

}

void Game::collectFreeTiles(std::vector<std::vector<int> > &freeTiles) {

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (!board[y][x].value) {
                std::vector<int> newEmpty {y, x};
                freeTiles.push_back(newEmpty);
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

            std::cout << " | ";
            if (!board[y][x].value)
                std::cout << std::setw(7);
            else
                std::cout << std::setw(4) << board[y][x].value;

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
    std::cout << "  Press the keys to start and continue."; endl(4);

    if (err) {
        std::cout << "  Invalid input. Please try again."; endl(2);
    }

    c = getch();

    c = toupper(c);

    switch(c) {

        case 'W':
            move(UP);
            break;
        case 'A':
            move(LEFT);
            break;
        case 'S':
            move(DOWN);
            break;
        case 'D':
            move(RIGHT);
            break;
        default:
            drawBoard();
            input(1);
            break;

    }

    unblockTiles();

}

bool Game::canMove() {

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (!board[y][x].value)
                return true;

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++) {
            if (testAdd(y + 1, x, board[y][x].value))
                return true;
            if (testAdd(y - 1, x, board[y][x].value))
                return true;
            if (testAdd(y, x + 1, board[y][x].value))
                return true;
            if (testAdd(y, x - 1, board[y][x].value))
                return true;
        }

    return false;

}

bool Game::testAdd(int y, int x, ull value) {

    if (y < 0 || y > 3 || x < 0 || x > 3)
        return false;

    return board[y][x].value == value;

}

void Game::unblockTiles() {

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            board[y][x].blocked = false;

}

void Game::move(Directions d) {

    switch (d) {

        case UP:

            for (int x = 0; x < 4; x++) {
                int y = 1;
                while (y < 4) {
                    if (board[y][x].value)
                        moveVertical(y, x, -1);
                    y++;
                }
            }
            break;

        case DOWN:

            for (int x = 0; x < 4; x++) {
                int y = 2;
                while (y >= 0) {
                    if (board[y][x].value)
                        moveVertical(y, x, 1);
                    y--;
                }
            }
            break;

        case LEFT:

            for (int y = 0; y < 4; y++) {
                int x = 1;
                while (x < 4) {
                    if (board[y][x].value)
                        moveHorizontal(y, x, -1);
                    x++;
                }
            }
            break;

        case RIGHT:

            for (int y = 0; y < 4; y++) {
                int x = 2;
                while (x >= 0) {
                    if (board[y][x].value)
                        moveHorizontal(y, x, 1);
                    x--;
                }
            }
            break;

        }
}

void Game::moveVertical(int y, int x, int k) {

    Tile &currentTile = board[y][x];
    Tile &targetTile = board[y + k][x];

    int A = currentTile.value;
    int B = targetTile.value;
    int C = currentTile.blocked;
    int D = targetTile.blocked;

    if (B && A == B && !C && !D) {

        currentTile.value = 0;
        targetTile.value *= 2;
        if (!win)
            win = targetTile.value == 2048 ? true : false;
        score += targetTile.value;
        targetTile.blocked = true;

        largestTile = largestTile < targetTile.value ? targetTile.value : largestTile;

        moved = true;

    }
    
    else if (A && !B) {

        targetTile.value = currentTile.value;
        currentTile.value = 0;

        moved = true;

    }

    if (k == 1 && y < 2)
        moveVertical(y + k, x, 1);
    else if (k == -1 && y > 1)
        moveVertical(y + k, x, -1);

}

void Game::moveHorizontal(int y, int x, int k) {

    Tile &currentTile = board[y][x];
    Tile &targetTile = board[y][x + k];

    int A = currentTile.value;
    int B = targetTile.value;
    int C = currentTile.blocked;
    int D = targetTile.blocked;

    if (B && A == B && !C && !D) {

        currentTile.value = 0;
        targetTile.value *= 2;
        if (!win)
            win = targetTile.value == 2048 ? true : false;
        score += targetTile.value;
        targetTile.blocked = true;

        largestTile = largestTile < targetTile.value ? targetTile.value : largestTile;

        moved = true;

    }

    else if (A && !B) {

        targetTile.value = currentTile.value;
        currentTile.value = 0;

        moved = true;
    
    }

    if (k == 1 && x < 2)
        moveHorizontal(y, x + k, 1);
    else if (k == -1 && x > 1)
        moveHorizontal(y, x + k, -1);

}

void Game::statistics() {

    std::cout << "  STATISTICS"; endl();
    std::cout << "  ----------"; endl();
    std::cout << "  Final score:       " << score; endl();
    std::cout << "  Largest Tile:      " << largestTile; endl();
    std::cout << "  Number of moves:   " << moveCount; endl();
    std::cout << "  Time taken:        " << duration << " seconds"; endl();

}

void Game::startGame() {

    auto startTime = std::chrono::high_resolution_clock::now();

    addTile();

    while (1) {

        if (moved)
            if(!addTile()) {
                drawBoard();
                break;
            }

        drawBoard();

        input();

    }

    auto finishTime = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finishTime - startTime;
    duration = elapsed.count();

    std::string msg = win ? "  You win!" : "  Game over! You lose.";
    std::cout << msg; endl(3);

    statistics();

}