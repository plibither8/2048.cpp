#include "menu.hpp"
#include "color.hpp"
#include "game-graphics.hpp"
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
  DrawAlways(std::cout, Game::Graphics::AsciiArt2048);
  Scoreboard::prettyPrintScoreboard(std::cout);
  Statistics::prettyPrintStats(std::cout);
  std::cout << std::flush;
  pause_for_keypress();
  Menu::startMenu();
}

std::string drawMainMenuGraphics(bool input_error_choice_invalid) {
  std::ostringstream str_os;
  DrawAlways(str_os, Game::Graphics::AsciiArt2048);
  DrawAlways(str_os, Game::Graphics::Menu::MainMenuTitlePrompt);
  DrawAlways(str_os, Game::Graphics::Menu::MainMenuOptionsPrompt);
  // Only outputs if there is an input error...
  DrawOnlyWhen(str_os, input_error_choice_invalid,
               Game::Graphics::Menu::InputMenuErrorInvalidInputPrompt);
  DrawAlways(str_os, Game::Graphics::Menu::InputMenuPrompt);
  return str_os.str();
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
  DrawAlways(std::cout,
             DataSuppliment(FlagInputErrornousChoice, drawMainMenuGraphics));
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
