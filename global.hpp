#pragma once

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "color.hpp"

#ifdef _WIN32
    #define CLEAR "cls"
    #include <conio.h>

#else
    #define CLEAR "clear"

    #include <unistd.h>
    #include <termios.h>

    char getch() {
        char buf = 0;
        struct termios old = {0};
        if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
        old.c_lflag &= ~ICANON;
        old.c_lflag &= ~ECHO;
        old.c_cc[VMIN] = 1;
        old.c_cc[VTIME] = 0;
        if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
        if (read(0, &buf, 1) < 0)
            perror("read()");
        old.c_lflag |= ICANON;
        old.c_lflag |= ECHO;
        if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");
        return (buf);
    }

#endif

typedef unsigned long long ull;

void endl(int n = 1) {

    for (int i = 0; i < n; i++) {
        std::cout << std::endl;
    }

};

void clearScreen() {
    system(CLEAR);
};

std::ostream &bold_on(std::ostream &os) {
    return os << "\e[1m";
}

std::ostream &bold_off(std::ostream &os) {
    return os << "\e[0m";
}

void drawAscii() {
    endl();
    std::cout << green << bold_on << "   /\\\\\\\\\\\\\\\\\\          /\\\\\\\\\\\\\\                /\\\\\\         /\\\\\\\\\\\\\\\\\\    " << bold_off << def; endl();
    std::cout << green << bold_on << "  /\\\\\\///////\\\\\\      /\\\\\\/////\\\\\\            /\\\\\\\\\\       /\\\\\\///////\\\\\\  " << bold_off << def; endl();
    std::cout << green << bold_on << "  \\///      \\//\\\\\\    /\\\\\\    \\//\\\\\\         /\\\\\\/\\\\\\      \\/\\\\\\     \\/\\\\\\  " << bold_off << def; endl();
    std::cout << green << bold_on << "             /\\\\\\/    \\/\\\\\\     \\/\\\\\\       /\\\\\\/\\/\\\\\\      \\///\\\\\\\\\\\\\\\\\\/   " << bold_off << def; endl();
    std::cout << green << bold_on << "           /\\\\\\//      \\/\\\\\\     \\/\\\\\\     /\\\\\\/  \\/\\\\\\       /\\\\\\///////\\\\\\  " << bold_off << def; endl();
    std::cout << green << bold_on << "         /\\\\\\//         \\/\\\\\\     \\/\\\\\\   /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   /\\\\\\      \\//\\\\\\ " << bold_off << def; endl();
    std::cout << green << bold_on << "        /\\\\\\/            \\//\\\\\\    /\\\\\\   \\///////////\\\\\\//   \\//\\\\\\      /\\\\\\  " << bold_off << def; endl();
    std::cout << green << bold_on << "        /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   \\///\\\\\\\\\\\\\\/              \\/\\\\\\      \\///\\\\\\\\\\\\\\\\\\/   " << bold_off << def; endl();
    std::cout << green << bold_on << "        \\///////////////      \\///////                \\///         \\/////////     " << bold_off << def; endl(4);
};