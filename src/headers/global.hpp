#ifndef GLOBAL_H
#define GLOBAL_H

#include <iosfwd>
#include <string>

using ull = unsigned long long;
void getKeypressDownInput(char &);

template<typename T>
void DrawAlways(std::ostream &os, T f) {
  os << f();
}

template<typename T>
void DrawOnlyWhen(std::ostream &os, bool trigger, T f) {
  if (trigger) {
    DrawAlways(os, f);
  }
}

template<typename T>
void DrawAsOneTimeFlag(std::ostream &os, bool &trigger, T f) {
  if (trigger) {
    DrawAlways(os, f);
    trigger = !trigger;
  }
}

void pause_for_keypress();
void wait_for_any_letter_input(std::istream &is);
void clearScreen();
void drawAscii();
std::string secondsFormat(double);

#endif
