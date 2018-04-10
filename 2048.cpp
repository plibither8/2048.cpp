#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <vector>

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
            
			std::vector< std::vector<int> > emptyArr;
			emptyTiles(emptyArr);

			if(!emptyArr.size())
				boardFull = true;

            int a, b;
            do {
                a = rand() % 4;
                b = rand() % 4;
            } while(board[a][b].value);

            int twoOrFour = rand() % 100;
			board[a][b].value = twoOrFour > 89 ? 4 : 2;

			moved = true;

        }

		void emptyTiles(std::vector<std::vector<int> > &emptyArr) {
			
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					if (!board[i][j].value) {
						std::vector<int> newEmpty{ i, j };
						emptyArr.push_back(newEmpty);
					}

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

        void input(int err = 0) {

            moved = true;
            char c;

            std::cout << "W => Up";
            endl(1);
            std::cout << "A => Left";
            endl(1);
            std::cout << "S => Down";
            endl(1);
            std::cout << "D => Right";
            endl(4);

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

        bool canMove() {
			
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					if (!board[i][j].value)
						return true;

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++) {
					if (testAdd(i + 1, j, board[i][j].value))
						return true;
					if (testAdd(i - 1, j, board[i][j].value))
						return true;
					if (testAdd(i, j + 1, board[i][j].value))
						return true;
					if (testAdd(i, j - 1, board[i][j].value))
						return true;
				}
			
			return false;
		
		}

		bool testAdd(int i, int j, uint value) {
		
			if (i < 0 || i > 3 || j < 0 || j > 3)
				return false;
		
			return board[i][j].value == value;
		
		}

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