#include "statistics.hpp"
#include "color.hpp"
#include "scores-graphics.hpp"
#include "scores.hpp"
#include "statistics-graphics.hpp"
#include <algorithm>
#include <array>
#include <fstream>
#include <sstream>

namespace Statistics {

namespace {

std::string receive_input_player_name(std::istream &is) {
  std::string name;
  is >> name;
  return name;
}

total_game_stats_t generateStatsFromInputData(std::istream &is) {
  total_game_stats_t stats;
  is >> stats;
  return stats;
}

bool generateFilefromStatsData(std::ostream &os, total_game_stats_t stats) {
  os << stats;
  return true;
}

bool saveToFileEndGameStatistics(std::string filename, total_game_stats_t s) {
  std::ofstream filedata(filename);
  return generateFilefromStatsData(filedata, s);
}

Scoreboard::Graphics::finalscore_display_data_t
make_finalscore_display_data(Scoreboard::Score finalscore) {
  const auto fsdd = std::make_tuple(
      std::to_string(finalscore.score), std::to_string(finalscore.largestTile),
      std::to_string(finalscore.moveCount), secondsFormat(finalscore.duration));
  return fsdd;
};

} // namespace

load_stats_status_t loadFromFileStatistics(std::string filename) {
  std::ifstream statistics(filename);
  if (statistics) {
    total_game_stats_t stats = generateStatsFromInputData(statistics);
    return load_stats_status_t{true, stats};
  }
  return load_stats_status_t{false, total_game_stats_t{}};
}

ull load_game_best_score() {
  total_game_stats_t stats;
  bool stats_file_loaded{};
  ull tempscore{0};
  std::tie(stats_file_loaded, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  if (stats_file_loaded) {
    tempscore = stats.bestScore;
  }
  return tempscore;
}

void saveEndGameStats(Scoreboard::Score finalscore) {
  total_game_stats_t stats;
  // Need some sort of stats data values only.
  // No need to care if file loaded successfully or not...
  std::tie(std::ignore, stats) =
      loadFromFileStatistics("../data/statistics.txt");
  stats.bestScore =
      stats.bestScore < finalscore.score ? finalscore.score : stats.bestScore;
  stats.gameCount++;
  stats.winCount = finalscore.win ? stats.winCount + 1 : stats.winCount;
  stats.totalMoveCount += finalscore.moveCount;
  stats.totalDuration += finalscore.duration;

  saveToFileEndGameStatistics("../data/statistics.txt", stats);
}

void CreateFinalScoreAndEndGameDataFile(std::ostream &os, std::istream &is,
                                        Scoreboard::Score finalscore) {
  const auto finalscore_display_data = make_finalscore_display_data(finalscore);
  DrawAlways(os, DataSuppliment(finalscore_display_data,
                                Scoreboard::Graphics::EndGameStatisticsPrompt));

  DrawAlways(os, Graphics::AskForPlayerNamePrompt);
  const auto name = receive_input_player_name(is);
  finalscore.name = name;

  Scoreboard::saveScore(finalscore);
  saveEndGameStats(finalscore);
  DrawAlways(os, Graphics::MessageScoreSavedPrompt);
}

} // namespace Statistics

using namespace Statistics;

std::istream &operator>>(std::istream &is, total_game_stats_t &s) {
  is >> s.bestScore >> s.gameCount >> s.winCount >> s.totalMoveCount >>
      s.totalDuration;
  return is;
}

std::ostream &operator<<(std::ostream &os, total_game_stats_t &s) {
  os << s.bestScore << "\n"
     << s.gameCount << "\n"
     << s.winCount << "\n"
     << s.totalMoveCount << "\n"
     << s.totalDuration;
  return os;
}
