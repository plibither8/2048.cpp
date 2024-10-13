#include "statistics-graphics.hpp"
#include "color.hpp"
#include <array>
#include <iomanip>
#include <sstream>

namespace Statistics {
namespace Graphics {

std::string AskForPlayerNamePrompt() {
    constexpr auto score_prompt_text = "Please enter your name to save this score: ";
    constexpr auto sp = "  ";
    std::ostringstream score_prompt_richtext;
    score_prompt_richtext << bold_on << sp << score_prompt_text << bold_off;
    return score_prompt_richtext.str();
}

std::string MessageScoreSavedPrompt() {
    constexpr auto score_saved_text = "Score saved!";
    constexpr auto sp = "  ";
    std::ostringstream score_saved_richtext;
    score_saved_richtext << "\n"
                         << green << bold_on << sp << score_saved_text << bold_off
                         << def << "\n";
    return score_saved_richtext.str();
}

std::string TotalStatisticsOverlay(total_stats_display_data_t tsdd) {
    constexpr auto stats_title_text = "STATISTICS";
    constexpr auto divider_text = "──────────";
    constexpr auto header_border_text = "┌────────────────────┬─────────────┐";
    constexpr auto footer_border_text = "└────────────────────┴─────────────┘";
    
    constexpr auto stats_attributes_text = std::array{"Best Score", "Game Count",
                                                        "Number of Wins", "Total Moves Played",
                                                        "Total Duration"};
    constexpr auto no_save_text = "No saved statistics.";
    constexpr auto any_key_exit_text = "Press any key to return to the main menu... ";
    constexpr auto sp = "  ";

    enum TotalStatsDisplayDataFields {
        IDX_DATA_AVAILABLE,
        IDX_BEST_SCORE,
        IDX_GAME_COUNT,
        IDX_GAME_WIN_COUNT,
        IDX_TOTAL_MOVE_COUNT,
        IDX_TOTAL_DURATION,
        MAX_TOTALSTATSDISPLAYDATA_INDEXES
    };

    std::ostringstream stats_richtext;
    const auto stats_file_loaded = std::get<IDX_DATA_AVAILABLE>(tsdd);
    
    if (stats_file_loaded) {
        constexpr auto num_of_stats_attributes_text = stats_attributes_text.size();
        auto data_stats = std::array<std::string, num_of_stats_attributes_text>{};

        // Load data stats from the input tuple
        data_stats = {
            std::get<IDX_BEST_SCORE>(tsdd),
            std::get<IDX_GAME_COUNT>(tsdd),
            std::get<IDX_GAME_WIN_COUNT>(tsdd),
            std::get<IDX_TOTAL_MOVE_COUNT>(tsdd),
            std::get<IDX_TOTAL_DURATION>(tsdd)
        };

        stats_richtext << green << bold_on << sp << stats_title_text << bold_off << def << "\n";
        stats_richtext << green << bold_on << sp << divider_text << bold_off << def << "\n";
        stats_richtext << sp << header_border_text << "\n";

        // Use structured binding for better readability
        for (size_t i = 0; i < num_of_stats_attributes_text; ++i) {
            stats_richtext << sp << "│ " << bold_on << std::left << std::setw(18)
                           << stats_attributes_text[i] << bold_off
                           << " │ " << std::right << std::setw(11)
                           << data_stats[i] << " │\n";
        }
        
        stats_richtext << sp << footer_border_text << "\n";
    } else {
        stats_richtext << sp << no_save_text << "\n";
    }

    stats_richtext << "\n\n\n";
    stats_richtext << sp << any_key_exit_text;

    return stats_richtext.str();
}

} // namespace Graphics
} // namespace Statistics
