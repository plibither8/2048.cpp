#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <iomanip>
#include <limits>
#include <vector>
#include <chrono>
#include <math.h>
#include "global.h"
#include "color.h"
#include "scores.h"
#include "statistics.h"

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
        std::vector<std::vector <Tile> > board;

        void initialiseBoardArray();
        bool addTile();
        void collectFreeTiles(std::vector<std::vector<int> > &freeTiles);
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

        Game() : win(false), moved(true), boardFull(false), rexit(false), score(0), moveCount(-2), largestTile(2) {}
        void startGame(int err = 0);

};

#endif
