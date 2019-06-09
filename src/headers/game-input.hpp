#ifndef GAMEINPUT_H
#define GAMEINPUT_H

#include <array>

namespace Game {
namespace Input {

namespace Keypress {
namespace Code {

enum { CODE_ESC = 27, CODE_LSQUAREBRACKET = '[' };

// Hotkey bindings:
// Style: ANSI (Arrow Keys)
enum {
  CODE_ANSI_TRIGGER_1 = CODE_ESC,
  CODE_ANSI_TRIGGER_2 = CODE_LSQUAREBRACKET
};
enum {
  CODE_ANSI_UP = 'A',
  CODE_ANSI_DOWN = 'B',
  CODE_ANSI_LEFT = 'D',
  CODE_ANSI_RIGHT = 'C'
};

// Style: WASD
enum {
  CODE_WASD_UP = 'W',
  CODE_WASD_DOWN = 'S',
  CODE_WASD_LEFT = 'A',
  CODE_WASD_RIGHT = 'D'
};

// Style: Vim
enum {
  CODE_VIM_UP = 'K',
  CODE_VIM_DOWN = 'J',
  CODE_VIM_LEFT = 'H',
  CODE_VIM_RIGHT = 'L'
};

enum {
  CODE_HOTKEY_ACTION_SAVE = 'Z',
  CODE_HOTKEY_ALTERNATE_ACTION_SAVE = 'P',
  CODE_HOTKEY_QUIT_ENDLESS_MODE = 'X',
  CODE_HOTKEY_CHOICE_NO = 'N',
  CODE_HOTKEY_CHOICE_YES = 'Y',
  CODE_HOTKEY_PREGAMEMENU_BACK_TO_MAINMENU = 0
};

} // namespace Code
} // namespace Keypress

enum IntendedMoveFlag {
  FLAG_MOVE_LEFT,
  FLAG_MOVE_RIGHT,
  FLAG_MOVE_UP,
  FLAG_MOVE_DOWN,
  MAX_NO_INTENDED_MOVE_FLAGS
};
using intendedmove_t = std::array<bool, MAX_NO_INTENDED_MOVE_FLAGS>;

bool check_input_ansi(char c, intendedmove_t &intendedmove);
bool check_input_vim(char c, intendedmove_t &intendedmove);
bool check_input_wasd(char c, intendedmove_t &intendedmove);

} // namespace Input
} // namespace Game

#endif
