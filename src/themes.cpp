#include "themes.hpp"
#include "menu.hpp"
#include <exception>

std::string Theme::themedOutput(ull value) {
  return themedOutputByIndex(int(log2(value) - 1));
}

std::string Theme::themedOutputByIndex(int index) {
  if (!themed_output_vector.empty())
    return themed_output_vector[index];
  return themed_output_function(index);
}

std::string Theme::menuentry() {
  std::ostringstream ret;
  ret << theme_name << ": ";
  for (int i = 0; i < 3; ++i)
    ret << themedOutputByIndex(i) << ", ";
  ret << "...";
  return ret.str();
}

Theme ThemeController::chooseTheme() {
  bool err = false;
  ull themeCount = registry.size();
  while ((theme_code > themeCount || theme_code < 1)) {
    clearScreen();
    drawAscii();

    if (err) {
      std::cout << red
                << "  Invalid input. Theme number should range from 1 to "
                << themeCount << "." << def;
      endl(2);
    }

    std::cout << bold_on;
    for (int i = 1; i <= themeCount; ++i)
      std::cout << "  " << i << ". " << registry[i - 1].menuentry()
                << std::endl;
    std::cout << "  Enter theme number: " << bold_off;

    std::cin >> theme_code;
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::int32_t>::max(), '\n');
    err = true;
  }
  return registry[theme_code - 1];
}

void ThemeController::addTheme(Theme xTheme) {
  for (auto &r : registry)
    if (r == xTheme)
      return;
  registry.push_back(xTheme);
}

std::vector<std::string> intv_to_strv(std::vector<int> intv) {
  std::vector<std::string> strv;
  strv.reserve(intv.size());
  for (auto &i : intv) {
    strv.push_back(std::to_string(i));
  }
  return strv;
}

void loadThemes(ThemeController &controller) {
  controller.addTheme(Theme(
      "Standard", [](int index) { return std::to_string(ull(2) << index); }));

  controller.addTheme(Theme("Elements", {"H", "He", "Li", "Be", "B", "C", "N",
                                         "O", "F", "Ne", "Na", "Ca", "Sc"}));
  controller.addTheme(Theme("Fibonacci Numbers", [](int index) {
    ull DP[index + 2];
    DP[0] = 1;
    DP[1] = 2;
    for (int i = 2; i <= index; ++i)
      DP[i] = DP[i - 1] + DP[i - 2];
    return std::to_string(DP[index]);
  }));
  controller.addTheme(Theme("Programming Languages",
                            {"C", "C++", "PHP", "C#", "Py", "Bash", "Java",
                             "SQL", "CSS", "HTML", "JS", "Go", "Rust"}));
}