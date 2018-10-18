#ifndef THEMES_H
#define THEMES_H

#include "color.hpp"
#include "global.hpp"
#include <cmath>
#include <limits>
#include <vector>
#include <sstream>
#include <string>

std::vector<std::string> intv_to_strv(std::vector<int> intv);

class Theme {
public:
  Theme() = default;
  Theme(std::string _theme_name, std::vector<std::string> _themed_output)
      : theme_name(std::move(_theme_name)),
        themed_output(std::move(_themed_output)){};
  Theme(std::string _theme_name, std::vector<int> _themed_output)
      : theme_name(std::move(_theme_name)),
        themed_output(intv_to_strv(_themed_output)){};
  bool operator==(const Theme &t) const {
    return themed_output == t.themed_output;
  }
  std::string themedOutput(ull value);
  std::string menuentry();

private:
  std::string theme_name;
  std::vector<std::string> themed_output;
};

class ThemeController {
public:
  ThemeController() = default;
  Theme chooseTheme();
  void addTheme(Theme xTheme);

private:
  int theme_code = 0;
  std::vector<Theme> registry;
};

void loadThemes(ThemeController &controller);

#endif
