#include <iostream>
#include <string>
#include <iomanip>
#include <time.h>

typedef unsigned int uint

class tile {

	public:
		tile(): val(0), blocked(false) {}
		uint val;
		bool blocked;

}

bool moved = true, win = false, boardFull = false;
uint score = 0;

void start() {

	addTile();

	while (1) {
		if (moved)
			addTile();
		drawBoard();
		if (done)
			break;
		input();
	}

}

void addTile();
void drawBoard();
void input();
bool canMove();
void move();

int main () {

}