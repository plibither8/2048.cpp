#include "saveresource.hpp"
#include "gameboard.hpp"
#include <fstream>
#include <filesystem>

namespace Game {
namespace Saver {
namespace {

/**
 * @brief Generates a file from the previous game's statistics data.
 * 
 * This function writes the score and move count of the given game board to the provided output stream.
 * The data is formatted as "score:moveCount]" to indicate the end of the statistics data.
 * 
 * @param os The output stream where the statistics data will be written.
 * @param gb The game board containing the statistics to be saved.
 * @return bool Returns true after successfully writing the statistics data.
 * 
 * @note The ']' character is used to signify the end of the statistics data.
 */
bool generateFilefromPreviousGameStatisticsData(std::ostream &os, const GameBoard &gb) {
  os << gb.score << ":" << MoveCountOnGameBoard(gb) << "]" << std::endl;
  return true;
}

/**
 * @brief Generates a file from the previous game's state data.
 * 
 * This function writes the state of the given game board to the provided output stream.
 * The data is formatted using the printStateOfGameBoard function.
 * 
 * @param os The output stream where the state data will be written.
 * @param gb The game board containing the state to be saved.
 * @return bool Returns true after successfully writing the state data.
 */
bool generateFilefromPreviousGameStateData(std::ostream &os, const GameBoard &gb) {
  os << printStateOfGameBoard(gb);
  return true;
}

/**
 * @brief Saves the previous game state and statistics to a file.
 * 
 * This function creates or appends to a file specified by the filename, saving both the game state and statistics.
 * The game state is written first, followed by the game statistics.
 * 
 * @param filename The name of the file where the game state and statistics will be saved.
 * @param gb The game board containing the state and statistics to be saved.
 */
void saveToFilePreviousGameStateData(std::string filename, const GameBoard &gb) {
  std::ofstream stateFile(filename, std::ios_base::app);
  generateFilefromPreviousGameStateData(stateFile, gb);
  generateFilefromPreviousGameStatisticsData(stateFile, gb);
}

/**
 * @brief Saves the current game state to a file.
 * 
 * This function checks for the existence of the directory and creates it if necessary.
 * It then removes any existing file with the specified filename before saving the 
 * current state and statistics of the provided game board to a new file. This ensures 
 * that only the most recent game state is saved.
 * 
 * @param gb The game board object containing the current state to be saved.
 * @param filename The name of the file where the game state will be saved. This file will
 *                 be located in the ../data/SavedGameFiles/ directory. If a file with this
 *                 name already exists, it will be deleted before saving the new state.
 * 
 * @note If the directory does not exist, it will be created. Ensure that the application
 *       has the necessary permissions to write to the specified location.
 */
void saveGamePlayState(GameBoard gb, const std::string& filename) {
  std::filesystem::path directory_path = "../data/SavedGameFiles/";

  if (!std::filesystem::exists(directory_path)) {
    std::filesystem::create_directories(directory_path);
  }

  const auto path_to_file_gd_state = directory_path / filename; // Using filesystem path concatenation for clarity
  std::remove(path_to_file_gd_state.c_str()); // Removing the file if it exists

  saveToFilePreviousGameStateData(path_to_file_gd_state, gb);
}

} // namespace
} // namespace Saver
} // namespace Game
