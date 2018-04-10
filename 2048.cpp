#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>

typedef unsigned int uint;
enum Directions {
    UP,
    DOWN,
    RIGHT,
    LEFT
};

#ifdef WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

class tile {

    public:
        tile(): value(0), blocked(false) {}
        uint value;
        bool blocked;

};

class game {

    private:
        bool moved;
        bool win;
        bool boardFull;
        uint score;

        void endl(int n) {

            for (int i = 0; i < n; i++)
                std::cout << std::endl;

        }

        void addTile() {
            
            int a, b;
            do {
                a = rand() % 4;
                b = rand() % 4;
            } while(!board[a][b].value);

            int twoOrFour = rand() % 100;

            twoOrFour > 89 ? board[a][b].value = 2 : board[a][b].value = 4;

        }

        void drawBoard() {

            system(CLEAR);

            std::cout << "SCORE: " << score;
            endl(1);
            std::cout << "-----------------------------";
            endl(4);

            for (int i = 0; i < 4; i++) {

                std::cout << " +------+------+------+------+" ;
                endl(1);

                for (int j = 0; j < 4; j++) {

                    std::cout << " | ";
                    if (!board[i][j].value)
                        std::cout << std::setw(7);
                    else
                        std::cout << std::setw(4) << board[i][j].value;

                }

                std::cout << "| ";
                endl(1);

            }

            std::cout << " +------+------+------+------+";
            endl(4);
        }

        void move(Directions d) {}

        void input(int err = 0) {

            moved = false;
            char c;

            std::cout << "W => Up";
            endl(1);
            std::cout << "A => Left";
            endl(1);
            std::cout << "S => Down";
            endl(1);
            std::cout << "D => Right";
            endl(2);

            if (err) {
                std::cout << "Invalid input. Please try again.";
                endl(2);
            }

            std::cout << "Enter Move: ";
            std::cin >> c;

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

            };

        }

        bool canMove() {}
    
    public:

        game() : win(false), moved(true), boardFull(false), score(0) {}
        
        void start() {

            addTile();

            while (1) {
                if (moved)
                    addTile();
                drawBoard();
                if (boardFull)
                    break;
                input();
            }

        }

        tile board[4][4];
       
};

int main () {

    srand(time(NULL));

    game g;

    g.start();

}