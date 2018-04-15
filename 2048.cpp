/*
 *
 * Game board array is defined as `board[y][x]`
 *
 * Game board will follow the following structure:
 * 'x' => x-axis OR the horizotal line OR rows
 * 'y' => y-axis OR the vertical line OR columns
 *
 * For example (zero-indexed):
 * board[2][0] refers to the 0th tile (or column) in 2nd
 * row as in this case, x = 0 and y = 2. The specific tile
 * is denoted the '@' symbol in the following gameboard
 *
 *		+------+------+------+------+
 *		|      |      |      |      |
 *		+------+------+------+------+
 *		|      |      |      |      |
 *		+------+------+------+------+
 *		|   @  |      |      |      |
 *		+------+------+------+------+
 *		|      |      |      |      |
 *		+------+------+------+------+
 *
 */

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

        bool addTile() {

			std::vector< std::vector<int> > freeTiles;
			collectFreeTiles(freeTiles);

			if(!freeTiles.size()) {
				boardFull = true;
			}

			std::vector<int> randomFreeTile = freeTiles.at(rand() % freeTiles.size());
			int x = randomFreeTile.at(0);
			int y = randomFreeTile.at(1);
			board[x][y].value = rand() % 100 > 89 ? 4 : 2;

			moved = true;

			return canMove();

        }

		void collectFreeTiles(std::vector<std::vector<int> > &freeTiles) {

			for (int x = 0; x < 4; x++)
				for (int y = 0; y < 4; y++)
					if (!board[x][y].value) {
						std::vector<int> newEmpty {x, y};
						freeTiles.push_back(newEmpty);
					}

		}

        void drawBoard() {

            system(CLEAR);

            std::cout << "SCORE: " << score; endl(1);
            std::cout << "-----------------------------"; endl(4);

            for (int i = 0; i < 4; i++) {

                std::cout << " +------+------+------+------+" ; endl(1);

                for (int j = 0; j < 4; j++) {

                    std::cout << " | ";
                    if (!board[i][j].value)
                        std::cout << std::setw(7);
                    else
                        std::cout << std::setw(4) << board[i][j].value;

                }

                std::cout << " | "; endl(1);

            }

            std::cout << " +------+------+------+------+"; endl(4);
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

		void move(Directions d) {

			switch (d) {

				case UP:

					for (int x = 0; x < 4; x++) {
						int y = 1;
						while (y < 4) {
							if (board[x][y].value)
								moveVertical(x, y, -1);
							y++;
						}
					}
					break;

				case DOWN:

					for (int x = 0; x < 4; x++) {
						int y = 2;
						while (y >= 0) {
							if (board[x][y].value)
								moveVertical(x, y, 1);
							y--;
						}
					}
					break;

				case LEFT:

					for (int y = 0; y < 4; y++) {
						int x = 1;
						while (x < 4) {
							if (board[x][y].value)
								moveHorizontal(x, y, -1);
							x++;
						}
					}
					break;

				case RIGHT:

					for (int y = 0; y < 4; y++) {
						int x = 2;
						while (x >= 0) {
							if (board[x][y].value)
								moveHorizontal(x, y, 1);
							x--;
						}
					}

				}
		}

		void moveVertical(int i, int j, int d) {



		}

		void moveHorizontal(int i, int j, int d) {



		}

	public:

        game() : win(false), moved(true), boardFull(false), score(0) {}

        int start() {

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

        }

        tile board[4][4];

};

int main () {

    srand(time(NULL));

    game g;

    g.start();

}