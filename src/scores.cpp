#include "scores.hpp"

bool compare(const Score &a, const Score &b) {
  return a.score < b.score;
};

void Scoreboard::prompt() {
  constexpr auto score_prompt_text =
      "Please enter your name to save this score: ";
  constexpr auto sp = "  ";

  std::ostringstream score_prompt_richtext;
  score_prompt_richtext << bold_on << sp << score_prompt_text << bold_off;

  std::cout << score_prompt_richtext.str();
  std::cin >> name;
}

void Scoreboard::writeToFile() {

  std::fstream scores("../data/scores.txt", std::ios_base::app);
  scores << std::endl
         << name << " " << score << " " << win << " " << moveCount << " "
         << largestTile << " " << duration;
  newline();
  scores.close();
}

void Scoreboard::printScore() {
  constexpr auto no_save_text = "No saved scores.";
  constexpr auto score_attributes_text = {
      "No.", "Name", "Score", "Won?", "Moves", "Largest Tile", "Duration"};
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

  readFile();

  clearScreen();
  drawAscii();
  str_os << green << bold_on << sp << score_title_text << bold_off << def
         << "\n";
  str_os << green << bold_on << sp << divider_text << bold_off << def << "\n";

  const auto number_of_scores = scoreList.size();
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

    auto counter{1};
    const auto print_score_stat = [&counter, &str_os](const Score i) {
      constexpr auto number_of_fields = 7;
      auto data_stats = std::array<std::string, number_of_fields>{};
      data_stats = {std::to_string(counter),     i.name,
                    std::to_string(i.score),     i.win ? "Yes" : "No",
                    std::to_string(i.moveCount), std::to_string(i.largestTile),
                    secondsFormat(i.duration)};
      str_os << sp << "│ " << std::setw(2) << data_stats[0] << ". │ "
             << std::left << std::setw(18) << data_stats[1] << std::right
             << " │ " << std::setw(8) << data_stats[2] << " │ " << std::setw(4)
             << data_stats[3] << " │ " << std::setw(5) << data_stats[4] << " │ "
             << std::setw(12) << data_stats[5] << " │ " << std::setw(12)
             << data_stats[6] << " │"
             << "\n";
      counter++;
    };

    std::for_each(std::begin(scoreList), std::end(scoreList), print_score_stat);
    str_os << sp << bottom_border_text << "\n";
  } else {
    str_os << sp << no_save_text << "\n";
  }
  str_os << "\n\n";
  std::cout << str_os.str();
}

void Scoreboard::printStats() {
  constexpr auto stats_title_text = "STATISTICS";
  constexpr auto divider_text = "──────────";
  constexpr auto header_border_text = "┌────────────────────┬─────────────┐";
  constexpr auto footer_border_text = "└────────────────────┴─────────────┘";
  constexpr auto stats_attributes_text = {
      "Best Score", "Game Count", "Number of Wins", "Total Moves Played",
      "Total Duration"};
  constexpr auto no_save_text = "No saved statistics.";
  constexpr auto any_key_exit_text = "Press any key to exit: ";
  constexpr auto sp = "  ";

  Stats stats;
  std::ostringstream stats_richtext;
  if (stats.collectStatistics()) {
    auto data_stats = std::array<std::string, stats_attributes_text.size()>{};
    data_stats = {
        std::to_string(stats.bestScore), std::to_string(stats.gameCount),
        std::to_string(stats.winCount), std::to_string(stats.totalMoveCount),
        secondsFormat(stats.totalDuration)};

    auto counter{0};
    const auto populate_stats_info = [data_stats, &counter,
                                      &stats_richtext](const std::string) {
      stats_richtext << sp << "│ " << bold_on << std::left << std::setw(18)
                     << std::begin(stats_attributes_text)[counter] << bold_off
                     << " │ " << std::right << std::setw(11)
                     << data_stats[counter] << " │"
                     << "\n";
      counter++;
    };

    stats_richtext << green << bold_on << sp << stats_title_text << bold_off
                   << def << "\n";
    stats_richtext << green << bold_on << sp << divider_text << bold_off << def
                   << "\n";
    stats_richtext << sp << header_border_text << "\n";
    std::for_each(std::begin(stats_attributes_text),
                  std::end(stats_attributes_text), populate_stats_info);
    stats_richtext << sp << footer_border_text << "\n";

  } else {
    stats_richtext << sp << no_save_text << "\n";
  }

  stats_richtext << "\n\n\n";
  stats_richtext << sp << any_key_exit_text;

  std::cout << stats_richtext.str();
  char c;
  std::cin >> c;
  exit(EXIT_SUCCESS);
}

void Scoreboard::padding(std::string name) {

  int length = name.length();
  while (18 - length++) {
    std::cout << " ";
  }
}

void Scoreboard::readFile() {

  std::ifstream scores("../data/scores.txt");
  if (scores.fail()) {
    return;
  }

  std::string playerName;
  ull playerScore;
  bool win;
  ull largestTile;
  long long moveCount;
  double duration;

  while (scores >> playerName >> playerScore >> win >> moveCount >>
         largestTile >> duration) {

    Score bufferScore;
    bufferScore.name = playerName;
    bufferScore.score = playerScore;
    bufferScore.win = win;
    bufferScore.largestTile = largestTile;
    bufferScore.moveCount = moveCount;
    bufferScore.duration = duration;

    scoreList.push_back(bufferScore);
  };

  std::sort(scoreList.begin(), scoreList.end(), compare);
}

void Scoreboard::save() {
  constexpr auto score_saved_text = "Score saved!";
  constexpr auto sp = "  ";
  std::ostringstream score_saved_richtext;
  score_saved_richtext << green << bold_on << sp << score_saved_text << bold_off
                       << def << "\n";

  prompt();
  writeToFile();
  std::cout << score_saved_richtext.str();
}
