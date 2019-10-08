#include "menu.hpp"
#include "color.hpp"
#include "game-graphics.hpp"
#include "game.hpp"
#include "global.hpp"
#include "menu-graphics.hpp"
#include "scores-graphics.hpp"
#include "scores.hpp"
#include "statistics-graphics.hpp"
#include "statistics.hpp"
#include <algorithm>
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

Scoreboard::Graphics::scoreboard_display_data_list_t
make_scoreboard_display_data_list() {
  using namespace Scoreboard::Graphics;
  auto scoreList = Scoreboard::Scoreboard_t{};
  // bool loaded_scorelist;
  // Warning: Does not care if file exists or not!
  std::tie(std::ignore, scoreList) =
      Scoreboard::loadFromFileScore("../data/scores.txt");

  auto counter{1};
  const auto convert_to_display_list_t = [&counter](const Scoreboard::Score s) {
    const auto data_stats = std::make_tuple(
        std::to_string(counter), s.name, std::to_string(s.score),
        s.win ? "Yes" : "No", std::to_string(s.moveCount),
        std::to_string(s.largestTile), secondsFormat(s.duration));
    counter++;
    return data_stats;
  };

  auto scoreboard_display_list = scoreboard_display_data_list_t{};
  std::transform(std::begin(scoreList), std::end(scoreList),
                 std::back_inserter(scoreboard_display_list),
                 convert_to_display_list_t);
  return scoreboard_display_list;
};

Statistics::Graphics::total_stats_display_data_t
make_total_stats_display_data() {
  Statistics::total_game_stats_t stats;
  bool stats_file_loaded{};
  std::tie(stats_file_loaded, stats) =
      Statistics::loadFromFileStatistics("../data/statistics.txt");

  const auto tsdd = std::make_tuple(
      stats_file_loaded, std::to_string(stats.bestScore),
      std::to_string(stats.gameCount), std::to_string(stats.winCount),
      std::to_string(stats.totalMoveCount), secondsFormat(stats.totalDuration));
  return tsdd;
};

void showScores() {
  using namespace Game::Graphics;
  using namespace Scoreboard::Graphics;
  using namespace Statistics::Graphics;
  const auto sbddl = make_scoreboard_display_data_list();
  const auto tsdd = make_total_stats_display_data();

  clearScreen();
  DrawAlways(std::cout, AsciiArt2048);
  DrawAlways(std::cout, DataSuppliment(sbddl, ScoreboardOverlay));
  DrawAlways(std::cout, DataSuppliment(tsdd, TotalStatisticsOverlay));
  std::cout << std::flush;
  pause_for_keypress();
  ::Menu::startMenu();
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
  DrawAlways(std::cout, Game::Graphics::AsciiArt2048);
  DrawAlways(std::cout,
             DataSuppliment(FlagInputErrornousChoice,
                            Game::Graphics::Menu::MainMenuGraphicsOverlay));
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
