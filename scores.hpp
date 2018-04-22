#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "global.hpp"
#include "statistics.hpp"

struct Score {
    std::string name;
    ull score;
    bool win;
    ull largestTile;
    long long moveCount;
    double duration;
};

bool compare(const Score &a, const Score &b) {
    return a.score < b.score;
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

void Scoreboard::prompt() {

    std::cout << "  Please enter your name: ";
    std::cin >> name;

}

void Scoreboard::writeToFile() {

    std::fstream scores("./data/scores.txt", std::ios_base::app);
    scores << std::endl << name << " " << score << " " << win << " " << moveCount << " " << largestTile << " " << duration; endl();
    scores.close();

}

void Scoreboard::printScore() {

    readFile();

    clearScreen();
    drawAscii();
    std::cout << green << bold_on << "  SCOREBOARD" << bold_off << def; endl();
    std::cout << green << bold_on << "  ----------" << bold_off << def; endl(2);

    int size = scoreList.size();

    for (int i = size - 1; i >= 0; i--) {

        std::string playerName = scoreList[i].name;
        ull playerScore = scoreList[i].score;
        std::string won = scoreList[i].win ? "Yes" : "No";
        long long moveCount = scoreList[i].moveCount;
        ull largestTile = scoreList[i].largestTile;
        double duration = scoreList[i].duration;

        if (i == size - 1) {
            std::cout << "  +-----+--------------------+----------+------+-------+--------------+--------------+"; endl();
            std::cout << "  | " << bold_on << "No." << bold_off << " | "
                        << bold_on << "Name" << bold_off << "               | "
                        << bold_on << "Score" << bold_off << "    | "
                        << bold_on << "Won?" << bold_off << " | "
                        << bold_on << "Moves" << bold_off << " | "
                        << bold_on << "Largest Tile" << bold_off << " | "
                        << bold_on << "Duration (s)" << bold_off << " |";
            endl();
            std::cout << "  +-----+--------------------+----------+------+-------+--------------+--------------+"; endl();
        }

        std::cout << "  | " << std::setw(2) << size - i << ". | " 
                            << playerName; padding(playerName);
        std::cout << " | " << std::setw(8) << playerScore << " | "
                            << std::setw(4) << won << " | "
                            << std::setw(5) << moveCount << " | "
                            << std::setw(12) << largestTile << " | "
                            << std::setw(12) << duration << " | ";
        endl();

    }

    if (!size) {
        std::cout << "  No saved scores.";
        endl();
    }
    else {
        std::cout << "  +-----+--------------------+----------+------+-------+--------------+--------------+";
    }

    endl(3);

}

void Scoreboard::printStats() {

    Stats stats;
    stats.collectStatistics();
    std::cout << green << bold_on << "  STATISTICS" << bold_off << def; endl();
    std::cout << green << bold_on << "  ----------" << bold_off << def; endl(2);
    std::cout << "  +--------------------+------------+"; endl();
    std::cout << "  | " << bold_on << "Best Score        " << bold_off << " | " << std::setw(10) << stats.bestScore << " |"; endl();
    std::cout << "  | " << bold_on << "Game Count        " << bold_off << " | " << std::setw(10) << stats.gameCount << " |"; endl();
    std::cout << "  | " << bold_on << "Number of Wins    " << bold_off << " | " << std::setw(10) << stats.winCount << " |"; endl();
    std::cout << "  | " << bold_on << "Total Moves Played" << bold_off << " | " << std::setw(10) << stats.totalMoveCount << " |"; endl();
    std::cout << "  | " << bold_on << "Total Duration (s)" << bold_off << " | " << std::setw(10) << stats.totalDuration << " |"; endl();
    std::cout << "  +--------------------+------------+"; endl();
    
    endl(3);

    std::cout << "  Press any key to exit: ";
    char c;
    std::cin >> c;
    exit(1);

}

void Scoreboard::padding(std::string name) {

    int length = name.length();
    while (18 - length++) {
        std::cout << " ";
    }

}

void Scoreboard::readFile() {

    std::ifstream scores("./data/scores.txt");

    std::string playerName;
    ull playerScore;
    bool win;
    ull largestTile;
    long long moveCount;
    double duration;

    while (scores >> playerName >> playerScore >> win >> moveCount >> largestTile >> duration) {

        Score bufferScore;
        bufferScore.name = playerName;
        bufferScore.score = playerScore;
        bufferScore.win = win;
        bufferScore.largestTile = largestTile;
        bufferScore.moveCount = moveCount;
        bufferScore.duration = duration;

        scoreList.push_back(bufferScore);

    };

    std::sort(scoreList.begin(), scoreList.end(), compare);

}

void Scoreboard::save() {

    prompt();
    writeToFile();
    std::cout << green << bold_on << "  Score saved!" << bold_off << def; endl();

}