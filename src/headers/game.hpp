#ifndef GAME_H
#define GAME_H

#include "gameboard.hpp"

namespace Game {

enum class PlayGameFlag { BrandNewGame, ContinuePreviousGame };
void playGame(PlayGameFlag cont, GameBoard gb,
              unsigned long long userInput_PlaySize = 1);

void startGame();
void continueGame();
}; // namespace Game

#endif
