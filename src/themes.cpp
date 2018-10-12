#include "themes.hpp"
#include "menu.hpp"

std::string Themes::themedOutput(ull value) {
  switch (themeCode) {
  case 1:
    return std::to_string(value);
  case 2:
    return elements[log2(value) - 1];
  case 3:
    return std::to_string(fibonacci_numbers[log2(value) - 1]);
  case 4:
    return programming_languages[log2(value) - 1];
  default:
    return "ERR";
  }
}

void Themes::chooseTheme() {
  bool err = false;
  while ((themeCode > themeCount || themeCode < 1)) {
    clearScreen();
    drawAscii();

    if (err) {
      std::cout << red
                << "  Invalid input. Theme number should range from 1 to "
                << themeCount << "." << def;
      endl(2);
    }

    std::cout << bold_on << "  1. Standard: 2, 4, 8, ..." << std::endl
              << "  2. Elements: H, He, Li, ..." << std::endl
              << "  3. Fibonacci Numbers: 1, 2, 3, ..." << std::endl
              << "  4. Programming Languages: C, C++, PHP, ..." << std::endl
              << "  Enter theme number: " << bold_off;

    std::cin >> themeCode;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::int32_t>::max(), '\n');
    err = true;
  }
}