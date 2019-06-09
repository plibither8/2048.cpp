#include "game-graphics.hpp"
#include "color.hpp"
#include <sstream>

namespace Game {
namespace Graphics {
std::string MessageScoreSavedPrompt() {
  constexpr auto score_saved_text = "Score saved!";
  constexpr auto sp = "  ";
  std::ostringstream score_saved_richtext;
  score_saved_richtext << "\n"
                       << green << bold_on << sp << score_saved_text << bold_off
                       << def << "\n";
  return score_saved_richtext.str();
}

std::string AskForPlayerNamePrompt() {
  constexpr auto score_prompt_text =
      "Please enter your name to save this score: ";
  constexpr auto sp = "  ";
  std::ostringstream score_prompt_richtext;
  score_prompt_richtext << bold_on << sp << score_prompt_text << bold_off;
  return score_prompt_richtext.str();
}

std::string BoardInputPrompt() {
  const auto board_size_prompt_text = {
      "(NOTE: Scores and statistics will be saved only for the 4x4 gameboard)\n",
      "Enter gameboard size - (Enter '0' to go back): "};
  constexpr auto sp = "  ";
  std::ostringstream board_size_prompt_richtext;
  board_size_prompt_richtext
      << bold_on << sp << std::begin(board_size_prompt_text)[0] << sp
      << std::begin(board_size_prompt_text)[1] << bold_off;
  return board_size_prompt_richtext.str();
}

std::string YouWinPrompt() {
  constexpr auto win_game_text = "You win! Congratulations!";
  constexpr auto sp = "  ";
  std::ostringstream win_richtext;
  win_richtext << green << bold_on << sp << win_game_text << def << bold_off
               << "\n\n\n";
  return win_richtext.str();
}

std::string GameOverPrompt() {
  constexpr auto lose_game_text = "Game over! You lose.";
  constexpr auto sp = "  ";
  std::ostringstream lose_richtext;
  lose_richtext << red << bold_on << sp << lose_game_text << def << bold_off
                << "\n\n\n";
  return lose_richtext.str();
}

std::string EndOfEndlessPrompt() {
  constexpr auto endless_mode_text =
      "End of endless mode! Thank you for playing!";
  constexpr auto sp = "  ";
  std::ostringstream endless_mode_richtext;
  endless_mode_richtext << red << bold_on << sp << endless_mode_text << def
                        << bold_off << "\n\n\n";
  return endless_mode_richtext.str();
}

std::string QuestionEndOfWinningGamePrompt() {
  constexpr auto win_but_what_next =
      "You Won! Continue playing current game? [y/n]";
  constexpr auto sp = "  ";
  std::ostringstream win_richtext;
  win_richtext << green << bold_on << sp << win_but_what_next << def << bold_off
               << ": ";
  return win_richtext.str();
}

std::string GameStateNowSavedPrompt() {
  constexpr auto state_saved_text =
      "The game has been saved. Feel free to take a break.";
  constexpr auto sp = "  ";
  std::ostringstream state_saved_richtext;
  state_saved_richtext << green << bold_on << sp << state_saved_text << def
                       << bold_off << "\n\n";
  return state_saved_richtext.str();
}

std::string GameBoardNoSaveErrorPrompt() {
  constexpr auto no_save_found_text =
      "No saved game found. Starting a new game.";
  constexpr auto sp = "  ";
  std::ostringstream no_save_richtext;
  no_save_richtext << red << bold_on << sp << no_save_found_text << def
                   << bold_off << "\n\n";
  return no_save_richtext.str();
}

std::string InvalidInputGameBoardErrorPrompt() {
  constexpr auto invalid_prompt_text = "Invalid input. Please try again.";
  constexpr auto sp = "  ";
  std::ostringstream invalid_prompt_richtext;
  invalid_prompt_richtext << red << sp << invalid_prompt_text << def << "\n\n";
  return invalid_prompt_richtext.str();
}

std::string BoardSizeErrorPrompt() {
  const auto invalid_prompt_text = {
      "Invalid input. Gameboard size should range from ", " to ", "."};
  //  constexpr auto num_of_invalid_prompt_text = 3;
  constexpr auto sp = "  ";
  std::ostringstream error_prompt_richtext;
  error_prompt_richtext << red << sp << std::begin(invalid_prompt_text)[0]
                        << MIN_GAME_BOARD_PLAY_SIZE
                        << std::begin(invalid_prompt_text)[1]
                        << MAX_GAME_BOARD_PLAY_SIZE
                        << std::begin(invalid_prompt_text)[2] << def << "\n\n";
  return error_prompt_richtext.str();
}

std::string InputCommandListPrompt() {
  constexpr auto sp = "  ";
  const auto input_commands_list_text = {
      "W or K or ↑ => Up", "A or H or ← => Left", "S or J or ↓ => Down",
      "D or L or → => Right", "Z or P => Save"};
  std::ostringstream str_os;
  for (const auto txt : input_commands_list_text) {
    str_os << sp << txt << "\n";
  }
  return str_os.str();
}

std::string EndlessModeCommandListPrompt() {
  constexpr auto sp = "  ";
  const auto endless_mode_list_text = {"X => Quit Endless Mode"};
  std::ostringstream str_os;
  for (const auto txt : endless_mode_list_text) {
    str_os << sp << txt << "\n";
  }
  return str_os.str();
}

std::string InputCommandListFooterPrompt() {
  constexpr auto sp = "  ";
  const auto input_commands_list_footer_text = {
      "", "Press the keys to start and continue.", "\n"};
  std::ostringstream str_os;
  for (const auto txt : input_commands_list_footer_text) {
    str_os << sp << txt << "\n";
  }
  return str_os.str();
}

} // namespace Graphics
} // namespace Game
