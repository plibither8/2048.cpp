#include "menu.hpp"
#include "color.hpp"
#include "game.hpp"
#include "global.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <array>
#include <iostream>
#include <sstream>

namespace {

enum MainMenuStatusFlag {
  FLAG_NULL,
  FLAG_START_GAME,
  FLAG_CONTINUE_GAME,
  FLAG_DISPLAY_HIGHSCORES,
  FLAG_EXIT_GAME,
  MAX_NO_MAIN_MENU_STATUS_FLAGS
};

using mainmenustatus_t = std::array<bool, MAX_NO_MAIN_MENU_STATUS_FLAGS>;

mainmenustatus_t mainmenustatus{};
bool FlagInputErrornousChoice{};

void startGame() {
  Game::startGame();
}

void continueGame() {
  Game::continueGame();
}

void showScores() {
  clearScreen();
  drawAscii();
  Scoreboard::prettyPrintScoreboard(std::cout);
  Statistics::prettyPrintStats(std::cout);
  std::cout << std::flush;
  pause_for_keypress();
  Menu::startMenu();
}

void drawMainMenuTitle(std::ostream &out_os) {
  constexpr auto greetings_text = "Welcome to ";
  constexpr auto gamename_text = "2048!";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream title_richtext;
  title_richtext << bold_on << sp << greetings_text << blue << gamename_text
                 << def << bold_off << "\n";

  str_os << title_richtext.str();
  out_os << str_os.str();
}

void drawMainMenuOptions(std::ostream &out_os) {
  const auto menu_list_txt = {"1. Play a New Game", "2. Continue Previous Game",
                              "3. View Highscores and Statistics", "4. Exit"};
  constexpr auto sp = "        ";

  std::ostringstream str_os;

  str_os << "\n";
  for (const auto txt : menu_list_txt) {
    str_os << sp << txt << "\n";
  }
  str_os << "\n";

  out_os << str_os.str();
}

void drawInputMenuErrorInvalidInput(std::ostream &out_os, bool err) {
  if (err) {
    constexpr auto err_input_text = "Invalid input. Please try again.";
    constexpr auto sp = "  ";

    std::ostringstream str_os;
    std::ostringstream err_input_richtext;
    err_input_richtext << red << sp << err_input_text << def << "\n\n";

    str_os << err_input_richtext.str();
    out_os << str_os.str();
  }
}

void drawInputMenuPrompt(std::ostream &out_os) {
  constexpr auto prompt_choice_text = "Enter Choice: ";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream prompt_choice_richtext;

  prompt_choice_richtext << sp << prompt_choice_text;

  str_os << prompt_choice_richtext.str();

  out_os << str_os.str();
}

void drawMainMenuGraphics(std::ostream &out_os) {
  drawAscii();
  drawMainMenuTitle(out_os);
  drawMainMenuOptions(out_os);
  // Only outputs if there is an input error...
  drawInputMenuErrorInvalidInput(out_os, FlagInputErrornousChoice);
  drawInputMenuPrompt(out_os);
}

void receive_input_flags(std::istream &in_os) {
  // Reset ErrornousChoice flag...
  FlagInputErrornousChoice = bool{};
  char c;
  in_os >> c;

  switch (c) {
  case '1':
    mainmenustatus[FLAG_START_GAME] = true;
    break;
  case '2':
    mainmenustatus[FLAG_CONTINUE_GAME] = true;
    break;
  case '3':
    mainmenustatus[FLAG_DISPLAY_HIGHSCORES] = true;
    break;
  case '4':
    mainmenustatus[FLAG_EXIT_GAME] = true;
    break;
  default:
    FlagInputErrornousChoice = true;
    break;
  }
}

void process_MainMenu() {
  if (mainmenustatus[FLAG_START_GAME]) {
    startGame();
  }
  if (mainmenustatus[FLAG_CONTINUE_GAME]) {
    continueGame();
  }
  if (mainmenustatus[FLAG_DISPLAY_HIGHSCORES]) {
    showScores();
  }
  if (mainmenustatus[FLAG_EXIT_GAME]) {
    exit(EXIT_SUCCESS);
  }
}

bool soloLoop() {
  // No choice in Menu selected, reset all flags...
  mainmenustatus = mainmenustatus_t{};
  clearScreen();
  drawMainMenuGraphics(std::cout);
  receive_input_flags(std::cin);
  process_MainMenu();
  return FlagInputErrornousChoice;
}

void endlessLoop() {
  while (soloLoop())
    ;
}

} // namespace

namespace Menu {

void startMenu() {
  endlessLoop();
}

} // namespace Menu
