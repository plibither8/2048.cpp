#include "game-pregamemenu.hpp"
#include "game-graphics.hpp"
#include "game-input.hpp"
#include "game.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include "loadresource.hpp"
#include "menu.hpp"
#include <array>
#include <iostream>
#include <limits>
#include <sstream>

namespace Game {

namespace PreGameSetup {

namespace {

enum PreGameSetupStatusFlag {
  FLAG_NULL,
  FLAG_START_GAME,
  FLAG_RETURN_TO_MAIN_MENU,
  MAX_NO_PREGAME_SETUP_STATUS_FLAGS
};

using pregamesetup_status_t =
    std::array<bool, MAX_NO_PREGAME_SETUP_STATUS_FLAGS>;

pregamesetup_status_t pregamesetup_status{};

ull stored_game_size{1};
bool FlagInputErrornousChoice{};
bool noSave{};

void process_PreGameMenu() {
  if (pregamesetup_status[FLAG_START_GAME]) {
    playGame(PlayGameFlag::BrandNewGame, GameBoard{stored_game_size},
             stored_game_size);
  }
  if (pregamesetup_status[FLAG_RETURN_TO_MAIN_MENU]) {
    Menu::startMenu();
  }
}

int Receive_Input_Playsize(std::istream &is) {
  int userInput_PlaySize{};
  if (!(is >> userInput_PlaySize)) {
    constexpr auto INVALID_INPUT_VALUE_FLAG = -1;
    userInput_PlaySize = INVALID_INPUT_VALUE_FLAG;
    is.clear();
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  return userInput_PlaySize;
}

void receive_input_flags(std::istream &is) {
  using namespace Input::Keypress::Code;

  // Reset ErrornousChoice flag...
  FlagInputErrornousChoice = bool{};

  // If not number, emits -1.
  // it should emit bool (valid / invalid value) and a valid number if ok input.
  // WARN:: "0-3" breaks this code!
  const auto c = Receive_Input_Playsize(is);

  const auto is_valid_game_size =
      (c >= MIN_GAME_BOARD_PLAY_SIZE) && (c <= MAX_GAME_BOARD_PLAY_SIZE);

  // Regular case;
  if (is_valid_game_size) {
    stored_game_size = c;
    pregamesetup_status[FLAG_START_GAME] = true;
  }

  // Special Case;
  bool is_not_special_case{};

  switch (c) {
  case CODE_HOTKEY_PREGAMEMENU_BACK_TO_MAINMENU:
    pregamesetup_status[FLAG_RETURN_TO_MAIN_MENU] = true;
    break;
  default:
    is_not_special_case = true;
    break;
  }

  if (!is_valid_game_size && is_not_special_case) {
    FlagInputErrornousChoice = true;
  }
}

bool soloLoop() {
  bool invalidInputValue = FlagInputErrornousChoice;
  const auto QuestionAboutBoardSizePrompt = [&invalidInputValue]() {
    std::ostringstream str_os;
    // Prints only if "invalidInputValue" is true
    DrawOnlyWhen(str_os, invalidInputValue, Graphics::BoardSizeErrorPrompt);
    DrawAlways(str_os, Graphics::BoardInputPrompt);
    return str_os.str();
  };

  pregamesetup_status = pregamesetup_status_t{};

  clearScreen();
  DrawAlways(std::cout, Game::Graphics::AsciiArt2048);
  DrawAsOneTimeFlag(std::cout, noSave, Graphics::GameBoardNoSaveErrorPrompt);
  DrawAlways(std::cout, QuestionAboutBoardSizePrompt);

  receive_input_flags(std::cin);
  process_PreGameMenu();
  return FlagInputErrornousChoice;
}

void endlessLoop() {
  while (soloLoop())
    ;
}

enum class NewGameFlag { NewGameFlagNull, NoPreviousSaveAvailable };

void SetUpNewGame(NewGameFlag ns) {
  noSave = (ns == NewGameFlag::NoPreviousSaveAvailable) ? true : false;
  endlessLoop();
}

/**
 * @brief Initializes a GameBoard object by loading game data from a specified file.
 *
 * This function constructs the full path to the game data file by prepending "../data/" to the given filename.
 * It then calls the `load_game` function to load the game data into a new GameBoard object. The function returns 
 * a tuple containing the status of the game load operation (true if successful, false otherwise) and the initialized 
 * GameBoard object.
 *
 * @param filename The name of the file from which to read the game data.
 * @return A tuple containing a boolean indicating the success of the load operation and the initialized GameBoard object.
 */
load_gameboard_status_t initialiseContinueBoardArray(const std::string& filename)
{
  using namespace Loader;
 // const auto gameboard_data_filename = "../data/" + filename;
  auto gb = GameBoard{1};
  auto loaded_game = load_game(filename, gb);
  return std::make_tuple(loaded_game, gb);
}


/**
 * @brief Continues a previously saved game from a specified file.
 * 
 * This function attempts to load the game state from the provided filename.
 * If successful, it continues the game using the loaded state. If the loading
 * fails, it sets up a new game indicating that no previous save is available.
 * 
 * @param filename The name of the file from which to load the previous game state.
 */
void DoContinueOldGame(const std::string& filename) {
  bool load_old_game_ok;
  GameBoard oldGameBoard;
  std::tie(load_old_game_ok, oldGameBoard) = initialiseContinueBoardArray(filename);
  if (load_old_game_ok) {
    playGame(PlayGameFlag::ContinuePreviousGame, oldGameBoard);
  } else {
    SetUpNewGame(NewGameFlag::NoPreviousSaveAvailable);
  }
}

} // namespace

void SetUpNewGame() {
  SetUpNewGame(NewGameFlag::NewGameFlagNull);
}

/**
 * @brief Continue a previously saved game.
 * 
 * The ContinueOldGame function has been updated to accept a filename directly.
 * This allows the user to load a specific save file instead of only the last saved game.
 * 
 * @param filename The name of the file containing the saved game to load.
 */
void ContinueOldGame(const std::string& filename) {
  DoContinueOldGame(filename);
}

} // namespace PreGameSetup

} // namespace Game
