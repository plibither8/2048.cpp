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

#ifdef WINDOWS
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

bool moved = true, win = false, boardFull = false;
uint score = 0;
tile board[4][4];

void endLine(int n) {

	for (int i = 0; i < n; i++)
		std::cout << std::endl;

}

void addTile() {};

void drawBoard() {

	system(CLEAR);

	std::cout << "SCORE: " << score;
	endLine(1);
	std::cout << "-----------------------------";
	endLine(4);

	for (int i = 0; i < 4; i++) {

		std::cout << "+------+------+------+------+" ;
		endLine(1);

		for (int j = 0; j < 4; j++) {

			std::cout << "| ";
			if (!board[i][j].value)
				std::cout << std::setw(7);
			else
				std::cout << std::setw(4) << board[i][j].value;

		}

		std::cout << " | ";
		endLine(1);

	}

	std::cout << "+------+------+------+------+";
	endLine(4);
};

void move(Directions d) {};

void input(int err = 0) {

	moved = false;
	char c;

	std::cout << "W => Up";
	endLine(1);
	std::cout << "A => Left";
	endLine(1);
	std::cout << "S => Down";
	endLine(1);
	std::cout << "D => Right";
	endLine(2);

	if (err) {
		std::cout << "Invalid input. Please try again.";
		endLine(2);
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

};

bool canMove() {};

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

int main () {

	srand(time(NULL));

	drawBoard();

	input();

}