#include "menu.hpp"

void Menu::startMenu(int err) {

  clearScreen();

  drawAscii();
  std::cout << bold_on << "  Welcome to " << blue << "2048!" << def << bold_off;
  newline(2);
  std::cout << "          1. Play a New Game";
  newline();
  std::cout << "          2. Continue Previous Game";
  newline();
  std::cout << "          3. View Highscores and Statistics";
  newline();
  std::cout << "          4. Exit";
  newline(2);

  input(err);
}

void Menu::input(int err) {

  if (err) {
    std::cout << red << "  Invalid input. Please try again." << def;
    newline(2);
  }

  std::cout << "  Enter Choice: ";
  char c;
  std::cin >> c;

  if (std::cin.eof()) {
    std::cout << std::endl;
    exit(EXIT_SUCCESS);
  }

  switch (c) {
  case '1':
    startGame();
    break;
  case '2':
    continueGame();
    break;
  case '3':
    showScores();
    break;
  case '4':
    exit(EXIT_SUCCESS);
  default:
    startMenu(1);
    break;
  }
}

void Menu::startGame() {

  Game g;
  g.startGame();
}

void Menu::continueGame() {
  Game g;
  g.continueGame();
}

void Menu::showScores() {

  Scoreboard s;
  s.printScore();
  s.printStats();
}

void drawAscii() {
  constexpr auto title_card_2048 = R"(
   /\\\\\\\\\          /\\\\\\\                /\\\         /\\\\\\\\\
  /\\\///////\\\      /\\\/////\\\            /\\\\\       /\\\///////\\\
  \///      \//\\\    /\\\    \//\\\         /\\\/\\\      \/\\\     \/\\\
             /\\\/    \/\\\     \/\\\       /\\\/\/\\\      \///\\\\\\\\\/
           /\\\//      \/\\\     \/\\\     /\\\/  \/\\\       /\\\///////\\\
         /\\\//         \/\\\     \/\\\   /\\\\\\\\\\\\\\\\   /\\\      \//\\\
        /\\\/            \//\\\    /\\\   \///////////\\\//   \//\\\      /\\\
        /\\\\\\\\\\\\\\\   \///\\\\\\\/              \/\\\      \///\\\\\\\\\/
        \///////////////      \///////                \///         \/////////
  )";
  std::cout << green << bold_on << title_card_2048 << bold_off << def;
  newline(3);
}
