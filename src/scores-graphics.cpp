#include "scores-graphics.hpp"
#include "color.hpp"
#include <iomanip>
#include <sstream>

namespace Scoreboard {
namespace Graphics {

std::string ScoreboardOverlay(scoreboard_display_data_list_t sbddl) {
  constexpr auto no_save_text = "No saved scores.";
  const auto score_attributes_text = {
      "No.", "Name", "Score", "Won?", "Moves", "Largest Tile", "Duration"};
  //  constexpr auto num_of_score_attributes_text = 7;
  constexpr auto header_border_text =
      "┌─────┬────────────────────┬──────────┬──────┬───────┬──────────────┬──────────────┐";
  constexpr auto mid_border_text =
      "├─────┼────────────────────┼──────────┼──────┼───────┼──────────────┼──────────────┤";
  constexpr auto bottom_border_text =
      "└─────┴────────────────────┴──────────┴──────┴───────┴──────────────┴──────────────┘";
  constexpr auto score_title_text = "SCOREBOARD";
  constexpr auto divider_text = "──────────";
  constexpr auto sp = "  ";

  std::ostringstream str_os;

  str_os << green << bold_on << sp << score_title_text << bold_off << def
         << "\n";
  str_os << green << bold_on << sp << divider_text << bold_off << def << "\n";

  const auto number_of_scores = sbddl.size();
  if (number_of_scores) {
    str_os << sp << header_border_text << "\n";
    str_os << std::left;
    str_os << sp << "│ " << bold_on << std::begin(score_attributes_text)[0]
           << bold_off << " │ " << bold_on << std::setw(18)
           << std::begin(score_attributes_text)[1] << bold_off << " │ "
           << bold_on << std::setw(8) << std::begin(score_attributes_text)[2]
           << bold_off << " │ " << bold_on
           << std::begin(score_attributes_text)[3] << bold_off << " │ "
           << bold_on << std::begin(score_attributes_text)[4] << bold_off
           << " │ " << bold_on << std::begin(score_attributes_text)[5]
           << bold_off << " │ " << bold_on << std::setw(12)
           << std::begin(score_attributes_text)[6] << bold_off << " │"
           << "\n";
    str_os << std::right;
    str_os << sp << mid_border_text << "\n";

    const auto print_score_stat = [&str_os](const scoreboard_display_data_t i) {
      str_os << sp << "│ " << std::setw(2) << std::get<0>(i) << ". │ "
             << std::left << std::setw(18) << std::get<1>(i) << std::right
             << " │ " << std::setw(8) << std::get<2>(i) << " │ " << std::setw(4)
             << std::get<3>(i) << " │ " << std::setw(5) << std::get<4>(i)
             << " │ " << std::setw(12) << std::get<5>(i) << " │ "
             << std::setw(12) << std::get<6>(i) << " │"
             << "\n";
    };

    for (const auto s : sbddl) {
      print_score_stat(s);
    }
    str_os << sp << bottom_border_text << "\n";
  } else {
    str_os << sp << no_save_text << "\n";
  }
  str_os << "\n\n";
  return str_os.str();
}

} // namespace Graphics

} // namespace Scoreboard
