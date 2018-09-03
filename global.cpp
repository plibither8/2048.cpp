#include "global.h"

#ifdef _WIN32

    void getInput(char &c) {
        std::cin >> c;
    }

#else
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

    void getInput(char &c) {
        c = getch();
    }

#endif

void endl(int n) {

    for (int i = 0; i < n; i++) {
        std::cout << std::endl;
    }

};

void clearScreen() {
    system("clear");
};
