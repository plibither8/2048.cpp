#include "game-input.hpp"
#include "global.hpp"

namespace Game {
namespace Input {
bool check_input_ansi(char c, intendedmove_t &intendedmove) {
  using namespace Keypress::Code;
  if (c == CODE_ANSI_TRIGGER_1) {
    getKeypressDownInput(c);
    if (c == CODE_ANSI_TRIGGER_2) {
      getKeypressDownInput(c);
      switch (c) {
      case CODE_ANSI_UP:
        intendedmove[FLAG_MOVE_UP] = true;
        return false;
      case CODE_ANSI_DOWN:
        intendedmove[FLAG_MOVE_DOWN] = true;
        return false;
      case CODE_ANSI_RIGHT:
        intendedmove[FLAG_MOVE_RIGHT] = true;
        return false;
      case CODE_ANSI_LEFT:
        intendedmove[FLAG_MOVE_LEFT] = true;
        return false;
      }
    }
  }
  return true;
}

bool check_input_vim(char c, intendedmove_t &intendedmove) {
  using namespace Keypress::Code;
  switch (toupper(c)) {
  case CODE_VIM_UP:
    intendedmove[FLAG_MOVE_UP] = true;
    return false;
  case CODE_VIM_LEFT:
    intendedmove[FLAG_MOVE_LEFT] = true;
    return false;
  case CODE_VIM_DOWN:
    intendedmove[FLAG_MOVE_DOWN] = true;
    return false;
  case CODE_VIM_RIGHT:
    intendedmove[FLAG_MOVE_RIGHT] = true;
    return false;
  }
  return true;
}

bool check_input_wasd(char c, intendedmove_t &intendedmove) {
  using namespace Keypress::Code;
  switch (toupper(c)) {
  case CODE_WASD_UP:
    intendedmove[FLAG_MOVE_UP] = true;
    return false;
  case CODE_WASD_LEFT:
    intendedmove[FLAG_MOVE_LEFT] = true;
    return false;
  case CODE_WASD_DOWN:
    intendedmove[FLAG_MOVE_DOWN] = true;
    return false;
  case CODE_WASD_RIGHT:
    intendedmove[FLAG_MOVE_RIGHT] = true;
    return false;
  }
  return true;
}
} // namespace Input
} // namespace Game
