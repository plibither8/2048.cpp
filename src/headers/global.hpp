#ifndef GLOBAL_H
#define GLOBAL_H

#include <string>

using ull = unsigned long long;
void getInput(char &);
void wait_for_any_letter_input(std::istream &is);
void newline(int n = 1);
void clearScreen();
void drawAscii();
std::string secondsFormat(double);

#endif
