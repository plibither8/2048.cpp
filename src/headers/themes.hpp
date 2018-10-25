#ifndef THEMES_H
#define THEMES_H

#include "color.hpp"
#include "global.hpp"
#include <cmath>
#include <functional>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

std::vector<std::string> intv_to_strv(std::vector<int> intv);

class Theme {
public:
  Theme() = default;
  Theme(std::string _theme_name, std::vector<std::string> _themed_output)
      : theme_name(std::move(_theme_name)),
        themed_output_vector(std::move(_themed_output)){};
  Theme(std::string _theme_name,
        std::function<std::string(int)> _themed_output_function)
      : theme_name(std::move(_theme_name)), themed_output_vector(),
        themed_output_function(std::move(_themed_output_function)){};
  bool operator==(const Theme &t) const {
    if (!themed_output_vector.empty())
      return themed_output_vector == t.themed_output_vector;
    return false;
  }
  std::string themedOutput(ull value);
  std::string themedOutputByIndex(int index);
  std::string menuentry();

private:
  std::string theme_name;
  std::vector<std::string> themed_output_vector;
  std::function<std::string(int)> themed_output_function;
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
