#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <chrono>

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
enum Directions {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

class tile {

    public:
        tile(): value(0), blocked(false) {}
        ull value;
        bool blocked;

};

class game {

    private:
        bool moved;
        bool win;
        bool boardFull;
        ull score;
        long long moveCount;
        double duration;

        void endl(int n = 1) {

            for (int i = 0; i < n; i++)
                std::cout << std::endl;

        }

        bool addTile() {

            std::vector< std::vector<int> > freeTiles;
            collectFreeTiles(freeTiles);

            if(!freeTiles.size()) {
                boardFull = true;
            }

            std::vector<int> randomFreeTile = freeTiles.at(rand() % freeTiles.size());
            int x = randomFreeTile.at(1);
            int y = randomFreeTile.at(0);
            board[y][x].value = rand() % 100 > 89 ? 4 : 2;

            moveCount++;
            moved = true;

            return canMove();

        }

        void collectFreeTiles(std::vector<std::vector<int> > &freeTiles) {

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                    if (!board[y][x].value) {
                        std::vector<int> newEmpty {y, x};
                        freeTiles.push_back(newEmpty);
                    }

        }

		void drawAscii() {
			endl(2);
			std::cout << "   /\\\\\\\\\\\\\\\\\\          /\\\\\\\\\\\\\\                /\\\\\\         /\\\\\\\\\\\\\\\\\\    "; endl();
			std::cout << "  /\\\\\\///////\\\\\\      /\\\\\\/////\\\\\\            /\\\\\\\\\\       /\\\\\\///////\\\\\\  "; endl();
			std::cout << "  \\///      \\//\\\\\\    /\\\\\\    \\//\\\\\\         /\\\\\\/\\\\\\      \\/\\\\\\     \\/\\\\\\  "; endl();
			std::cout << "             /\\\\\\/    \\/\\\\\\     \\/\\\\\\       /\\\\\\/\\/\\\\\\      \\///\\\\\\\\\\\\\\\\\\/   "; endl();
			std::cout << "           /\\\\\\//      \\/\\\\\\     \\/\\\\\\     /\\\\\\/  \\/\\\\\\       /\\\\\\///////\\\\\\  "; endl();
			std::cout << "         /\\\\\\//         \\/\\\\\\     \\/\\\\\\   /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   /\\\\\\      \\//\\\\\\ "; endl();
			std::cout << "        /\\\\\\/            \\//\\\\\\    /\\\\\\   \\///////////\\\\\\//   \\//\\\\\\      /\\\\\\  "; endl();
			std::cout << "        /\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\   \\///\\\\\\\\\\\\\\/              \\/\\\\\\      \\///\\\\\\\\\\\\\\\\\\/   "; endl();
			std::cout << "        \\///////////////      \\///////                \\///         \\/////////     "; endl(4);
		}

        void drawBoard() {

            system(CLEAR);

			drawAscii();
			std::cout << "  +---------------------------+"; endl();
			std::cout << "  | SCORE:" << std::setw(19) << score << " |"; endl();
            std::cout << "  | MOVES:" << std::setw(19) << moveCount << " |"; endl();
            std::cout << "  +---------------------------+"; endl(2);

            for (int y = 0; y < 4; y++) {

                std::cout << "  +------+------+------+------+" ; endl();
				std::cout << " ";

                for (int x = 0; x < 4; x++) {

                    std::cout << " | ";
                    if (!board[y][x].value)
                        std::cout << std::setw(7);
                    else
                        std::cout << std::setw(4) << board[y][x].value;

                }

                std::cout << " | "; endl();

            }

            std::cout << "  +------+------+------+------+"; endl(3);
        }

        void input(int err = 0) {

            moved = false;
            char c;

            std::cout << "  W => Up"; endl();
            std::cout << "  A => Left"; endl();
            std::cout << "  S => Down"; endl();
            std::cout << "  D => Right"; endl(2);
            std::cout << "  Press the keys to start and continue."; endl(4);

            if (err) {
                std::cout << "  Invalid input. Please try again."; endl(2);
            }

            c = getch();

            c = toupper(c);

            switch(c) {

                case 'W':
                    move(UP);
                    break;
                case 'A':
                    move(LEFT);
                    break;
                case 'S':
                    move(DOWN);
                    break;
                case 'D':
                    move(RIGHT);
                    break;
                default:
                    drawBoard();
                    input(1);
                    break;

            }

            unblockTiles();

        }

        bool canMove() {

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                    if (!board[y][x].value)
                        return true;

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++) {
                    if (testAdd(y + 1, x, board[y][x].value))
                        return true;
                    if (testAdd(y - 1, x, board[y][x].value))
                        return true;
                    if (testAdd(y, x + 1, board[y][x].value))
                        return true;
                    if (testAdd(y, x - 1, board[y][x].value))
                        return true;
                }

            return false;

        }

        bool testAdd(int y, int x, ull value) {

            if (y < 0 || y > 3 || x < 0 || x > 3)
                return false;

            return board[y][x].value == value;

        }

        void unblockTiles() {

            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                    board[y][x].blocked = false;

        }

        void move(Directions d) {

            switch (d) {

                case UP:

                    for (int x = 0; x < 4; x++) {
                        int y = 1;
                        while (y < 4) {
                            if (board[y][x].value)
                                moveVertical(y, x, -1);
                            y++;
                        }
                    }
                    break;

                case DOWN:

                    for (int x = 0; x < 4; x++) {
                        int y = 2;
                        while (y >= 0) {
                            if (board[y][x].value)
                                moveVertical(y, x, 1);
                            y--;
                        }
                    }
                    break;

                case LEFT:

                    for (int y = 0; y < 4; y++) {
                        int x = 1;
                        while (x < 4) {
                            if (board[y][x].value)
                                moveHorizontal(y, x, -1);
                            x++;
                        }
                    }
                    break;

                case RIGHT:

                    for (int y = 0; y < 4; y++) {
                        int x = 2;
                        while (x >= 0) {
                            if (board[y][x].value)
                                moveHorizontal(y, x, 1);
                            x--;
                        }
                    }
                    break;

                }
        }

        void moveVertical(int y, int x, int k) {

            tile &currentTile = board[y][x];
            tile &targetTile = board[y + k][x];

            int A = currentTile.value;
            int B = targetTile.value;
            int C = currentTile.blocked;
            int D = targetTile.blocked;

            if (B && A == B && !C && !D) {

                currentTile.value = 0;
                targetTile.value *= 2;
                win = targetTile.value == 2048 ? true : false;
                score += targetTile.value;
                targetTile.blocked = true;

                moved = true;

            } else if (A && !B) {

                targetTile.value = currentTile.value;
                currentTile.value = 0;

                moved = true;

            }

            if (k == 1 && y < 2)
                moveVertical(y + k, x, 1);
            else if (k == -1 && y > 1)
                moveVertical(y + k, x, -1);

        }

        void moveHorizontal(int y, int x, int k) {

            tile &currentTile = board[y][x];
            tile &targetTile = board[y][x + k];

            int A = currentTile.value;
            int B = targetTile.value;
            int C = currentTile.blocked;
            int D = targetTile.blocked;

            if (B && A == B && !C && !D) {

                currentTile.value = 0;
                targetTile.value *= 2;
                win = targetTile.value == 2048 ? true : false;
                score += targetTile.value;
                targetTile.blocked = true;

                moved = true;

            }

            else if (A && !B) {

                targetTile.value = currentTile.value;
                currentTile.value = 0;

                moved = true;
            
            }

            if (k == 1 && x < 2)
                moveHorizontal(y, x + k, 1);
            else if (k == -1 && x > 1)
                moveHorizontal(y, x + k, -1);

        }

        ull largestTile() {
            ull max = 2;
            
            for (int y = 0; y < 4; y++)
                for (int x = 0; x < 4; x++)
                    max = board[y][x].value > max ? board[y][x].value : max;
            
            return max;
        }

		void statistics() {

			std::cout << "  STATISTICS"; endl();
			std::cout << "  ----------"; endl();
            std::cout << "  Final score:       " << score; endl();
            std::cout << "  Largest tile:      " << largestTile(); endl();
            std::cout << "  Number of moves:   " << moveCount; endl();
            std::cout << "  Time taken:        " << duration << " seconds"; endl(2);

		}

    public:

        game() : win(false), moved(true), boardFull(false), score(0), moveCount(-2) {}

        void start() {

            auto start = std::chrono::high_resolution_clock::now();

            addTile();

            while (1) {

                if (moved)
                    if(!addTile()) {
                        drawBoard();
                        break;
                    }

                drawBoard();

                input();

            }

            auto finish = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double> elapsed = finish - start;
            duration = elapsed.count();

            std::string msg = win ? "  You win!" : "  Game over! You lose.";
            std::cout << msg; endl(3);

            statistics();

        }

        tile board[4][4];

};

int main () {

    srand(time(NULL));

    game g;

    g.start();

}