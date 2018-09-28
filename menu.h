#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "global.h"
#include "game.h"

class Menu {

    private:
        void startGame();
        void showScores();
        void printMenu();
        void input(int err);
	void continueGame();

    public:
        void startMenu(int err = 0);

};

void drawAscii();

#endif
