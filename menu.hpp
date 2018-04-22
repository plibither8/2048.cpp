#pragma once

#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include "game.hpp"
#include "scores.hpp"
#include "global.hpp"

class Menu {

	private:
		void startGame();
		void showScores();
		void printMenu();
		void input(int err);

	public:
		void startMenu(int err = 0);

};

void Menu::startMenu(int err) {

	clearScreen();

	drawAscii();
	std::cout << bold_on << "  Welcome to " << blue << "2048!" << def << bold_off; endl(2);
	std::cout << "  	1. Play a New Game"; endl();
	std::cout << "  	2. View Highscores and Statistics"; endl(2);

	input(err);

}

void Menu::input(int err) {

	if (err) {
		std::cout << red << "  Invalid input. Please try again." << def; endl(2);
	}

	std::cout << "  Enter Choice: ";
	char c;
	std::cin >> c;

	switch(c) {
		case '1':
			startGame();
			break;
		case '2':
			showScores();
			break;
		default:
			startMenu(1);
			break;
	}

}

void Menu::startGame() {

	Game g;
	g.startGame();

}

void Menu::showScores() {

	Scoreboard s;
	s.printScore();
	s.printStats();
	return;

}