#ifndef THEMES_H
#define THEMES_H

#include "color.hpp"
#include "global.hpp"
#include <cmath>
#include <limits>
#include <vector>

class Themes {
public:
  Themes()
      : fibonacci_numbers{1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 377, 610},
        elements{"H", "He", "Li", "Be", "B",  "C", "N",
                 "O", "F",  "Ne", "Na", "Ca", "Sc"},
        programming_languages{"C",   "C++", "PHP",  "C#", "Py", "Bash", "Java",
                              "SQL", "CSS", "HTML", "JS", "Go", "Rust"} {}
  std::string themedOutput(ull value);
  void chooseTheme();

private:
  int themeCode = 0;
  const int themeCount = 4;
  std::vector<std::string> elements;
  std::vector<std::string> programming_languages;
  std::vector<int> fibonacci_numbers;
};

#endif
