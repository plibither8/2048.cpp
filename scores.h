#ifndef SCORES_H
#define SCORES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "global.h"
#include "statistics.h"
#include "menu.h"


struct Score {
    std::string name;
    ull score;
    bool win;
    ull largestTile;
    long long moveCount;
    double duration;
};

class Scoreboard {
    private:
        std::string name;
        std::vector<Score> scoreList;
        void prompt();
        void writeToFile();
        void readFile();
        void padding(std::string name);

    public:
        ull score = 0;
        bool win;
        ull largestTile;
        long long moveCount;
        double duration;
        void printScore();
        void printStats();
        void save();
};
#endif
