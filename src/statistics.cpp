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

// Helper function to receive player name input
std::string receive_input_player_name(std::istream &is) {
    std::string name;
    is >> name;

    // Validate input
    if (is.fail()) {
        throw std::runtime_error("Invalid player name input.");
    }
    
    return name;
}

// Generate total game stats from input data
total_game_stats_t generateStatsFromInputData(std::istream &is) {
    total_game_stats_t stats;
    is >> stats;

    // Validate input
    if (is.fail()) {
        throw std::runtime_error("Invalid statistics input data.");
    }
    
    return stats;
}

// Save statistics data to the output stream
bool generateFilefromStatsData(std::ostream &os, const total_game_stats_t &stats) {
    os << stats;
    return os.good(); // Check if the write was successful
}

// Save end game statistics to a file
bool saveToFileEndGameStatistics(const std::string &filename, const total_game_stats_t &s) {
    std::ofstream filedata(filename);
    if (!filedata) {
        throw std::runtime_error("Could not open file for saving statistics.");
    }
    return generateFilefromStatsData(filedata, s);
}

// Create final score display data
Scoreboard::Graphics::finalscore_display_data_t make_finalscore_display_data(const Scoreboard::Score &finalscore) {
    return std::make_tuple(
        std::to_string(finalscore.score),
        std::to_string(finalscore.largestTile),
        std::to_string(finalscore.moveCount),
        secondsFormat(finalscore.duration)
    );
}

} // namespace

// Load statistics from a file
load_stats_status_t loadFromFileStatistics(const std::string &filename) {
    std::ifstream statistics(filename);
    if (!statistics) {
        return load_stats_status_t{false, total_game_stats_t{}};
    }

    total_game_stats_t stats = generateStatsFromInputData(statistics);
    return load_stats_status_t{true, stats};
}

// Load the best score from game statistics
ull load_game_best_score() {
    total_game_stats_t stats;
    bool stats_file_loaded{};
    ull tempscore{0};

    std::tie(stats_file_loaded, stats) = loadFromFileStatistics("../data/statistics.txt");
    if (stats_file_loaded) {
        tempscore = stats.bestScore;
    }

    return tempscore;
}

// Save end game statistics
void saveEndGameStats(const Scoreboard::Score &finalscore) {
    total_game_stats_t stats;
    std::tie(std::ignore, stats) = loadFromFileStatistics("../data/statistics.txt");
    
    stats.bestScore = std::max(stats.bestScore, finalscore.score);
    stats.gameCount++;
    stats.winCount += finalscore.win ? 1 : 0;
    stats.totalMoveCount += finalscore.moveCount;
    stats.totalDuration += finalscore.duration;

    saveToFileEndGameStatistics("../data/statistics.txt", stats);
}

// Create final score and end game data file
void CreateFinalScoreAndEndGameDataFile(std::ostream &os, std::istream &is, Scoreboard::Score finalscore) {
    const auto finalscore_display_data = make_finalscore_display_data(finalscore);
    DrawAlways(os, DataSuppliment(finalscore_display_data, Scoreboard::Graphics::EndGameStatisticsPrompt));

    DrawAlways(os, Graphics::AskForPlayerNamePrompt);
    finalscore.name = receive_input_player_name(is);
    
    Scoreboard::saveScore(finalscore);
    saveEndGameStats(finalscore);
    DrawAlways(os, Graphics::MessageScoreSavedPrompt);
}

} // namespace Statistics

using namespace Statistics;

// Overload operator to read total game stats
std::istream &operator>>(std::istream &is, total_game_stats_t &s) {
    is >> s.bestScore >> s.gameCount >> s.winCount >> s.totalMoveCount >> s.totalDuration;

    // Validate stream state after reading
    if (is.fail()) {
        throw std::runtime_error("Failed to read total game stats.");
    }

    return is;
}

// Overload operator to write total game stats
std::ostream &operator<<(std::ostream &os, const total_game_stats_t &s) {
    os << s.bestScore << "\n"
       << s.gameCount << "\n"
       << s.winCount << "\n"
       << s.totalMoveCount << "\n"
       << s.totalDuration;

    return os;
}
