#include "menu.hpp"
#include "color.hpp"
#include "game-graphics.hpp"
#include "game.hpp"
#include "gameboard.hpp" // To iterate over all saved Games
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
#include <vector>
#include <filesystem> //To use std::filestream

mainmenustatus_t mainmenustatus{};

/**
 * @brief Lists all saved game states in the specified directory.
 * 
 * This function scans the specified directory for regular files and returns their names
 * as a list of strings. It checks if the directory exists before attempting to list the files.
 * 
 * @param directory The directory to search for saved game state files.
 * @return std::vector<std::string> A vector containing the names of the saved game state files.
 */
std::vector<std::string> listSavedGameStates(const std::string& directory)
{
  std::vector<std::string> gameStates;

  if(!std::filesystem::exists(directory))
  {
    std::cout << "Directory does not exists." << std::endl;
    return gameStates;
  }

  for(auto& data : std::filesystem::directory_iterator(directory))
  {
    if(data.is_regular_file())
    {
      gameStates.push_back(data.path().filename().string());
    }
  }

  return gameStates;
}

/**
 * @brief Prompts the user to choose a game state from a list.
 * 
 * This function displays the available game states and prompts the user to select one.
 * If no game states are found, or if the user's choice is invalid, appropriate messages are displayed.
 * 
 * @param gamestate A vector containing the names of available game states.
 * @return std::string The name of the chosen game state, or an empty string if the choice is invalid.
 */
std::string chooseGameState(const std::vector<std::string>& gamestate)
{
  if(gamestate.empty())
  {
    std::cout << "No saved games found." << std::endl;
    return "";
  }

  std::cout << "Saved games are:" << std::endl;
  for (std::size_t i = 0; i < gamestate.size(); i++)
  {
    std::cout << i + 1 << ". " << gamestate[i] << std::endl;
  }

  unsigned int index;
  std::cout << "Choose game state:" << std::endl;
  std::cout << std::endl;
  std::cin >> index;

  if(index < 1 || index > gamestate.size())
  {
    std::cout << "Invalid choice." << std::endl;
    return "";
  }

  return gamestate[index - 1];
}

namespace {

bool FlagInputErrornousChoice{};

void startGame() {
  Game::startGame();
}

/**
 * @brief Continues a previously saved game.
 * 
 * This function prompts the user to choose a saved game state from a specified directory.
 * If a valid saved game state is selected, it continues the game using that state.
 * If no valid saved game state is selected, it notifies the user that the file is empty.
 * 
 * @note Changes in the new version:
 * - Added functionality to list and choose a saved game state from the specified directory.
 * - Validates the chosen game state file and continues the game using the selected file.
 * - Prints a message if the selected file is empty.
 */
void continueGame()
{
  std::string directory_state = "../data/SavedGameFiles/";
  std::string file_gb_state = chooseGameState(listSavedGameStates(directory_state));
  if (!file_gb_state.empty())
  {
    Game::continueGame(directory_state + file_gb_state);
  }
  else
  {
    std::cout << "The file is empty" << std::endl;
  }
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

/**
 * @brief Runs the endless loop until the game is exited.
 * 
 * This function continuously runs the solo game loop until the exit flag is set in the main menu status.
 * Initially, it sets the start menu flag to display the menu. If the start menu flag is set, it calls the solo loop
 * function to handle the menu interactions.
 * 
 * @note Changes in the new version:
 * - Added a condition to exit the loop when the exit flag is set in the main menu status.
 * - Integrated the menu start state and menu interaction within the loop.
 */
void endlessLoop() {
  // As long as the exit option is not selected in the menu
  while (!mainmenustatus[FLAG_EXIT_GAME])
  {
    // Initial state is Menu
    mainmenustatus[FLAG_START_MENU] = true;
    // If the menu flag is set, we enter...
    if (mainmenustatus[FLAG_START_MENU] == true)    
    {
      // ... the soloLoop() function, where we navigate the menu
      soloLoop();                         
    }                                               
  }
}

} // namespace

namespace Menu {

void startMenu() {
  endlessLoop();
}

} // namespace Menu
