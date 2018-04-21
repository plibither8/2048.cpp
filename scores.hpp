#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include "global.hpp"
#include "menu.hpp"

class Score {
    private:
        std::string name;
        void prompt();
        void writeToFile();
    public:
        ull score = 0;
        void readFile();
        void save();
};

void Score::prompt() {

    std::cout << "  Please enter your name: ";
    std::cin >> name;

}

void Score::writeToFile() {

    std::fstream scores("scores.txt", std::fstream::in | std::fstream::out | std::fstream::trunc);
    scores << name << " " << score; endl();
    scores.close();

}

void Score::readFile() {

    std::ifstream scores("scores.txt");
    
    std::string a;
    ull b;
    int count = 0;

    while (scores >> a >> b) {
        count++;
        if (count == 1) {
            clearScreen();
            drawAscii();
            std::cout << green << bold_on << "  HIGHSCORES" << bold_off << def; endl();
            std::cout << green << bold_on << "  ----------" << bold_off << def; endl(2);
        }
        std::cout << "  " << count << ". " << a << ": " << b; endl();
    }

    if (!count) {
        std::cout << "  No saved scores."; endl();
    }

    endl(1);

    std::cout << "  Press any key to exit: ";
    char c;
    std::cin >> c;
    exit(1);

}

void Score::save() {

    prompt();
    writeToFile();

}