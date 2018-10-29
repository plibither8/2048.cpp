#ifndef COLOR_H
#define COLOR_H

#include <ostream>

namespace Color {
enum class Code {
  BOLD = 1,
  RESET = 0,
  BG_BLUE = 44,
  BG_DEFAULT = 49,
  BG_GREEN = 42,
  BG_RED = 41,
  FG_BLACK = 30,
  FG_BLUE = 34,
  FG_CYAN = 36,
  FG_DARK_GRAY = 90,
  FG_DEFAULT = 39,
  FG_GREEN = 32,
  FG_LIGHT_BLUE = 94,
  FG_LIGHT_CYAN = 96,
  FG_LIGHT_GRAY = 37,
  FG_LIGHT_GREEN = 92,
  FG_LIGHT_MAGENTA = 95,
  FG_LIGHT_RED = 91,
  FG_LIGHT_YELLOW = 93,
  FG_MAGENTA = 35,
  FG_RED = 31,
  FG_WHITE = 97,
  FG_YELLOW = 33,
};

class Modifier {

  Code code;

public:
  Modifier(Code pCode) : code(pCode) {}
  friend std::ostream &operator<<(std::ostream &os, const Modifier &mod) {
    return os << "\033[" << static_cast<int>(mod.code) << "m";
  }
};

} // namespace Color

static Color::Modifier bold_off(Color::Code::RESET);
static Color::Modifier bold_on(Color::Code::BOLD);
static Color::Modifier def(Color::Code::FG_DEFAULT);
static Color::Modifier red(Color::Code::FG_RED);
static Color::Modifier green(Color::Code::FG_GREEN);
static Color::Modifier yellow(Color::Code::FG_YELLOW);
static Color::Modifier blue(Color::Code::FG_BLUE);
static Color::Modifier magenta(Color::Code::FG_MAGENTA);
static Color::Modifier cyan(Color::Code::FG_CYAN);
static Color::Modifier lightGray(Color::Code::FG_LIGHT_GRAY);
static Color::Modifier darkGray(Color::Code::FG_DARK_GRAY);
static Color::Modifier lightRed(Color::Code::FG_LIGHT_RED);
static Color::Modifier lightGreen(Color::Code::FG_LIGHT_GREEN);
static Color::Modifier lightYellow(Color::Code::FG_LIGHT_YELLOW);
static Color::Modifier lightBlue(Color::Code::FG_LIGHT_BLUE);
static Color::Modifier lightMagenta(Color::Code::FG_LIGHT_MAGENTA);
static Color::Modifier lightCyan(Color::Code::FG_LIGHT_CYAN);

#endif
