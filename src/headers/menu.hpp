#ifndef MENU_H
#define MENU_H

#include "game.hpp"
#include "global.hpp"
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <string>

class Menu {

private:
  void input_language(int err);
  void startGame();
  void showScores();
  void printMenu();
  void input(int err);
  void continueGame();

public:
  void startMenu(int err = 0);
  void selectLanguage(int err = 0);
};

void drawAscii();

#endif
