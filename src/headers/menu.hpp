#ifndef MENU_H
#define MENU_H

#include "gameboard.hpp"
#include <array>

/**
 * @enum MainMenuStatusFlag
 * @brief Enumeration of possible main menu status flags.
 *
 * These flags represent various states and actions that can be triggered from
 * the main menu.
 */
enum MainMenuStatusFlag {
  FLAG_NULL, /**< No action */
  FLAG_START_GAME, /**< Start a new game */
  FLAG_START_MENU, /**< Start menu (new flag for starting menu) */
  FLAG_CONTINUE_GAME, /**< Continue an existing game */
  FLAG_DISPLAY_HIGHSCORES, /**< Display the high scores */
  FLAG_EXIT_GAME, /**< Exit the game */
  MAX_NO_MAIN_MENU_STATUS_FLAGS /**< Maximum number of main menu status flags */
};

/**
 * @typedef mainmenustatus_t
 * @brief Array type for main menu status flags.
 *
 * This type represents an array of boolean values corresponding to each main
 * menu status flag.
 */
using mainmenustatus_t = std::array<bool, MAX_NO_MAIN_MENU_STATUS_FLAGS>;

/**
 * @var mainmenustatus
 * @brief Global variable to track the status of main menu flags.
 *
 * This variable is used to store the current status of each main menu flag,
 * allowing the game's main menu state to be accessed and modified globally.
 */
extern mainmenustatus_t mainmenustatus;

std::vector<std::string> listSavedGameStates(const std::string &directory);

std::string chooseGameState(const std::vector<std::string> &gamestate);

namespace Menu {
void startMenu();
} // namespace Menu

#endif
