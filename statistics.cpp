#include "statistics.h"

void Stats::collectStatistics() {

    std::ifstream statistics("./data/statistics.txt");

    statistics >> bestScore >> gameCount >> winCount >> totalMoveCount >> totalDuration;
}
