#ifndef STATISTICS_H
#define STATISTICS_H

#include <fstream>
#include "global.h"


class Stats {

    public:
        void collectStatistics();
        ull bestScore;
        ull totalMoveCount;
        int gameCount;
        double totalDuration;
        int winCount;

};

#endif
