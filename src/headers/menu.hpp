#ifndef MENU_H
#define MENU_H

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
