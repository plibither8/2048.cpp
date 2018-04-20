#pragma once

#include <ostream>

namespace Color {
    enum Code {
        FG_DEFAULT = 39,
        FG_BLACK = 30,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_YELLOW = 33,
        FG_BLUE = 34,
        FG_MAGENTA = 35,
        FG_CYAN = 36,
        FG_LIGHT_GRAY = 37,
        FG_DARK_GRAY = 90,
        FG_LIGHT_RED = 91,
        FG_LIGHT_GREEN = 92,
        FG_LIGHT_YELLOW = 93,
        FG_LIGHT_BLUE = 94,
        FG_LIGHT_MAGENTA = 95,
        FG_LIGHT_CYAN = 96,
        FG_WHITE = 97,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_BLUE = 44,
        BG_DEFAULT = 49
    };

    class Modifier {

        Code code;

        public:
            Modifier(Code pCode) : code(pCode) {}
            friend std::ostream &
            operator<<(std::ostream &os, const Modifier &mod) {
                return os << "\033[" << mod.code << "m";
            }
    };

}

Color::Modifier def(Color::FG_DEFAULT);
Color::Modifier red(Color::FG_RED);
Color::Modifier green(Color::FG_GREEN);
Color::Modifier yellow(Color::FG_YELLOW);
Color::Modifier blue(Color::FG_BLUE);
Color::Modifier magenta(Color::FG_MAGENTA);
Color::Modifier cyan(Color::FG_CYAN);
Color::Modifier lightGray(Color::FG_LIGHT_GRAY);
Color::Modifier darkGray(Color::FG_DARK_GRAY);
Color::Modifier lightRed(Color::FG_LIGHT_RED);
Color::Modifier lightGreen(Color::FG_LIGHT_GREEN);
Color::Modifier lightYellow(Color::FG_LIGHT_YELLOW);
Color::Modifier lightBlue(Color::FG_LIGHT_BLUE);
Color::Modifier lightMagenta(Color::FG_LIGHT_MAGENTA);
Color::Modifier lightCyan(Color::FG_LIGHT_CYAN);