#include "themes.hpp"
#include "menu.hpp"

std::string Theme::themedOutput(ull value) {
  return themed_output[log2(value) - 1];
}

std::string Theme::menuentry() {
  std::ostringstream ret;
  ret << theme_name << ": ";
  for (int i = 0; i < 3; ++i)
    ret << themed_output[i] << ", ";
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
  controller.addTheme(Theme("Standard", {2, 4, 8, 16, 32, 64, 128, 256, 512,
                                         1024, 2048, 4096, 8192}));

  controller.addTheme(Theme("Elements", {"H", "He", "Li", "Be", "B", "C", "N",
                                         "O", "F", "Ne", "Na", "Ca", "Sc"}));
  controller.addTheme(Theme(
      "Fibonacci Numbers", {1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 377, 610}));
  controller.addTheme(Theme("Programming Languages",
                            {"C", "C++", "PHP", "C#", "Py", "Bash", "Java",
                             "SQL", "CSS", "HTML", "JS", "Go", "Rust"}));
}