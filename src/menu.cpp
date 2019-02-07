#include "menu.hpp"
#include "color.hpp"
#include "game.hpp"
#include "global.hpp"
#include "scores.hpp"
#include <iostream>
#include <sstream>

namespace {

void startGame() {
  Game g;
  g.startGame();
}

void continueGame() {
  Game g;
  g.continueGame();
}

void showScores() {
  Scoreboard s;
  s.printScore();
  s.printStats();
}

void input(int err) {
  using Menu::startMenu;
  constexpr auto err_input_text = "Invalid input. Please try again.";
  constexpr auto prompt_choice_text = "Enter Choice: ";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream err_input_richtext;
  err_input_richtext << red << sp << err_input_text << def << "\n\n";
  std::ostringstream prompt_choice_richtext;
  prompt_choice_richtext << sp << prompt_choice_text;

  if (err) {
    str_os << err_input_richtext.str();
  }

  str_os << prompt_choice_richtext.str();
  std::cout << str_os.str();
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

} // namespace

namespace Menu {

void startMenu(int err) {
  constexpr auto greetings_text = "Welcome to ";
  constexpr auto gamename_text = "2048!";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream title_richtext;
  title_richtext << bold_on << sp << greetings_text << blue << gamename_text
                 << def << bold_off << "\n";

  constexpr auto menu_entry_text = R"(
          1. Play a New Game
          2. Continue Previous Game
          3. View Highscores and Statistics
          4. Exit

)";

  clearScreen();
  drawAscii();
  str_os << title_richtext.str();
  str_os << menu_entry_text;
  std::cout << str_os.str();
  input(err);
}

} // namespace Menu
