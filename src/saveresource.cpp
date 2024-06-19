#include "saveresource.hpp"
#include "gameboard.hpp"
#include <fstream>

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
bool generateFilefromPreviousGameStatisticsData(std::ostream &os,
                                                const GameBoard &gb) {
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
bool generateFilefromPreviousGameStateData(std::ostream &os,
                                           const GameBoard &gb) {
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
void saveToFilePreviousGameStateData(std::string filename,
                                     const GameBoard &gb) {
  std::ofstream stateFile(filename, std::ios_base::app);
  generateFilefromPreviousGameStateData(stateFile, gb);
  generateFilefromPreviousGameStatisticsData(stateFile, gb);

}

// This function can be deleted because it is no longer needed.
// The functionality to save game statistics has been integrated
// into the saveToFilePreviousGameStateData function, making this
// function redundant.
/*void saveToFilePreviousGameStatisticsData(std::string filename,
                                          const GameBoard &gb) {
  std::ofstream stats(filename, std::ios_base::app);
  generateFilefromPreviousGameStatisticsData(stats, gb);
}*/

} // namespace

/**
 * @brief Saves the current game state to a file.
 * 
 * This function removes any existing file with the specified filename and then
 * saves the current state and statistics of the provided game board to a new file.
 * 
 * @param gb The current game board to be saved.
 * @param filename The name of the file where the game state will be saved.
 */
void saveGamePlayState(GameBoard gb, const std::string& filename) {
  const auto path_to_file_gd_state = "../data/SavedGameFiles/" + filename;
  std::remove(path_to_file_gd_state.c_str());

  saveToFilePreviousGameStateData(path_to_file_gd_state, gb);
}

} // namespace Saver
} // namespace Game
