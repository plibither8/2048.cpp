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

  readFile();

  clearScreen();
  drawAscii();
  std::cout << green << bold_on << "  SCOREBOARD" << bold_off << def;
  newline();
  std::cout << green << bold_on << "  ──────────" << bold_off << def;
  newline();

  int size = scoreList.size();

  for (int i = size - 1; i >= 0; i--) {

    std::string playerName = scoreList[i].name;
    ull playerScore = scoreList[i].score;
    std::string won = scoreList[i].win ? "Yes" : "No";
    long long moveCount = scoreList[i].moveCount;
    ull largestTile = scoreList[i].largestTile;
    double duration = scoreList[i].duration;

    if (i == size - 1) {
      std::cout << "  "
                   "┌─────┬────────────────────┬──────────┬──────┬───────┬─────"
                   "─────────┬──────────────┐";
      newline();
      std::cout << "  │ " << bold_on << "No." << bold_off << " │ " << bold_on
                << "Name" << bold_off << "               │ " << bold_on
                << "Score" << bold_off << "    │ " << bold_on << "Won?"
                << bold_off << " │ " << bold_on << "Moves" << bold_off << " │ "
                << bold_on << "Largest Tile" << bold_off << " │ " << bold_on
                << "Duration    " << bold_off << " │";
      newline();
      std::cout << "  "
                   "├─────┼────────────────────┼──────────┼──────┼───────┼─────"
                   "─────────┼──────────────┤";
      newline();
    }

    std::cout << "  │ " << std::setw(2) << size - i << ". │ " << playerName;
    padding(playerName);
    std::cout << " │ " << std::setw(8) << playerScore << " │ " << std::setw(4)
              << won << " │ " << std::setw(5) << moveCount << " │ "
              << std::setw(12) << largestTile << " │ " << std::setw(12)
              << secondsFormat(duration) << " │ ";
    newline();
  }

  if (!size) {
    std::cout << "  No saved scores.";
    newline();
  } else {
    std::cout << "  "
                 "└─────┴────────────────────┴──────────┴──────┴───────┴───────"
                 "───────┴──────────────┘";
  }

  newline(3);
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
