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

  Stats stats;
  if (stats.collectStatistics()) {

    std::cout << green << bold_on << "  STATISTICS" << bold_off << def;
    newline();
    std::cout << green << bold_on << "  ──────────" << bold_off << def;
    newline();
    std::cout << "  ┌────────────────────┬─────────────┐";
    newline();
    std::cout << "  │ " << bold_on << "Best Score        " << bold_off << " │ "
              << std::setw(11) << stats.bestScore << " │";
    newline();
    std::cout << "  │ " << bold_on << "Game Count        " << bold_off << " │ "
              << std::setw(11) << stats.gameCount << " │";
    newline();
    std::cout << "  │ " << bold_on << "Number of Wins    " << bold_off << " │ "
              << std::setw(11) << stats.winCount << " │";
    newline();
    std::cout << "  │ " << bold_on << "Total Moves Played" << bold_off << " │ "
              << std::setw(11) << stats.totalMoveCount << " │";
    newline();
    std::cout << "  │ " << bold_on << "Total Duration    " << bold_off << " │ "
              << std::setw(11) << secondsFormat(stats.totalDuration) << " │";
    newline();
    std::cout << "  └────────────────────┴─────────────┘";
    newline();

  } else {

    std::cout << "  No saved statistics.";
    newline();
  }

  newline(3);

  std::cout << "  Press any key to exit: ";
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
