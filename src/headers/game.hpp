#ifndef GAME_H
#define GAME_H

#include "color.hpp"
#include "gameboard.hpp"
#include "global.hpp"
#include "point2d.hpp"
#include "scores.hpp"
#include "statistics.hpp"
#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <random>
#include <sstream>
#include <string>
#include <vector>

enum Directions { UP, DOWN, RIGHT, LEFT };

class Game {

private:
  ull bestScore;
  double duration;
  GameBoard gamePlayBoard;
  RandInt randInt;
  bool stateSaved;
  bool noSave;

  enum ContinueStatus { STATUS_END_GAME = 0, STATUS_CONTINUE = 1 };
  enum KeyInputErrorStatus { STATUS_INPUT_VALID = 0, STATUS_INPUT_ERROR = 1 };
  enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

  bool get_and_process_game_stats_string_data(std::istream &stats_file);
  bool load_game_stats_from_file(std::string filename);
  bool initialiseContinueBoardArray();
  void drawBoard() const;
  void drawScoreBoard(std::ostream &out_stream) const;
  void input(KeyInputErrorStatus err = STATUS_INPUT_VALID);
  void decideMove(Directions);

  bool collaspeTiles(point2D_t pt, point2D_t pt_offset);
  bool shiftTiles(point2D_t pt, point2D_t pt_offset);
  bool collasped_or_shifted_tiles(point2D_t pt, point2D_t pt_offset);
  bool check_recursive_offset_in_game_bounds(point2D_t pt, point2D_t pt_offset);

  void move(point2D_t pt, point2D_t pt_offset);
  void statistics() const;
  void saveStats() const;
  void saveScore() const;
  void saveState() const;
  void playGame(ContinueStatus);
  ull setBoardSize();

public:
  Game() : bestScore(0), duration(0.0), stateSaved(false), noSave(false) {}
  void startGame();
  void continueGame();
};

#endif
