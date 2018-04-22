#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include "global.hpp"


class Stats {

    public:
        void collectStatistics();
        ull bestScore;
        ull totalMoveCount;
        int gameCount;
        double totalDuration;
        int winCount;

};

void Stats::collectStatistics() {

    std::ifstream statistics("./data/statistics.txt");

    statistics >> bestScore >> gameCount >> winCount >> totalMoveCount >> totalDuration;
}