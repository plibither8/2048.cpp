#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "global.hpp"

struct Score {
    std::string name;
    ull score;
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
        void printScore();
        void save();
};

void Scoreboard::prompt() {

    std::cout << "  Please enter your name: ";
    std::cin >> name;

}

void Scoreboard::writeToFile() {

    std::fstream scores("./data/scores.txt", std::ios_base::app);
    scores << std::endl << name << " " << score; endl();
    scores.close();

}

void Scoreboard::printScore() {

    readFile();

    int size = scoreList.size();

    for (int i = size - 1; i >= 0; i--) {

        std::string playerName = scoreList[i].name;
        ull playerScore = scoreList[i].score;

        if (i == size - 1) {
            std::cout << "  +-----+--------------------+----------+"; endl();
            std::cout << "  | " << bold_on << "No." << bold_off << " | "
                        << bold_on << "Name" << bold_off << "               | "
                        << bold_on << "Score" << bold_off << "    |";
            endl();
            std::cout << "  +-----+--------------------+----------+";
            endl();
        }

        std::cout << "  | " << std::setw(2) << size - i << ". | " << playerName;
        padding(playerName);
        std::cout << " | " << std::setw(8) << playerScore << " |";
        endl();
    }

    if (!size) {
        std::cout << "  No saved scores.";
        endl();
    }
    else {
        std::cout << "  +-----+--------------------+----------+";
    }

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
    int count = 0;
    clearScreen();
    drawAscii();
    std::cout << green << bold_on << "  HIGHSCORES" << bold_off << def; endl();
    std::cout << green << bold_on << "  ----------" << bold_off << def; endl(2);


    while (scores >> playerName >> playerScore) {

        Score bufferScore;
        bufferScore.name = playerName;
        bufferScore.score = playerScore;

        scoreList.push_back(bufferScore);

    };

    std::sort(scoreList.begin(), scoreList.end(), compare);

}

void Scoreboard::save() {

    prompt();
    writeToFile();
    std::cout << green << bold_on << "  Scoreboard saved!" << bold_off << def; endl();

}