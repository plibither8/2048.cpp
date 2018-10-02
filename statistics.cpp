#include "statistics.h"

Stats::Stats()
    : bestScore(0)
    , totalMoveCount(0)
    , gameCount(0)
    , totalDuration(0.0)
    , winCount(0) {}

void Stats::collectStatistics() {

    std::ifstream statistics("./data/statistics.txt");
    if (statistics.fail()) {
        return;
    }

    statistics >> bestScore >> gameCount >> winCount >> totalMoveCount >> totalDuration;
}
