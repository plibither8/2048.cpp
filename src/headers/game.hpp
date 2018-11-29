#ifndef GAME_H
#define GAME_H

#include "gameboard.hpp"
#include "global.hpp"
#include <array>

enum Directions { UP, DOWN, RIGHT, LEFT };

class Game {
private:
  enum ContinueStatus { STATUS_END_GAME = 0, STATUS_CONTINUE = 1 };
  enum KeyInputErrorStatus { STATUS_INPUT_VALID = 0, STATUS_INPUT_ERROR = 1 };
  enum { COMPETITION_GAME_BOARD_PLAY_SIZE = 4 };

  enum GameStatusFlag { FLAG_WIN, FLAG_END_GAME, FLAG_SAVED_GAME, MAX_NO_GAME_STATUS_FLAGS };

  using gamestatus_t = std::array<bool, MAX_NO_GAME_STATUS_FLAGS>;

  gamestatus_t gamestatus{};

  ull bestScore;
  double duration;
  GameBoard gamePlayBoard;
  RandInt randInt;
  bool noSave;
  KeyInputErrorStatus input_err{STATUS_INPUT_VALID};

  bool get_and_process_game_stats_string_data(std::istream &stats_file);
  bool load_game_stats_from_file(std::string filename);
  bool initialiseContinueBoardArray();
  bool soloGameLoop();
  void drawEndScreen();
  void drawBoard() const;
  void drawGameState();
  void drawScoreBoard(std::ostream &out_stream) const;
  void input();
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

  void drawGraphics();
  void endlessGameLoop();
  void process_gamelogic();
  void drawInputControls();

public:
  Game() : bestScore(0), duration(0.0), noSave(false) {}
  void startGame();
  void continueGame();
};

#endif
