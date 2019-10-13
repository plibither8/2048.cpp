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

template<typename suppliment_t>
struct DataSupplimentInternalType {
  suppliment_t suppliment_data;
  template<typename function_t>
  std::string operator()(function_t f) const {
    return f(suppliment_data);
  }
};

template<typename suppliment_t, typename function_t>
auto DataSuppliment(suppliment_t needed_data, function_t f) {
  using dsit_t = DataSupplimentInternalType<suppliment_t>;
  const auto lambda_f_to_return = [=]() {
    const dsit_t depinject_func = dsit_t{needed_data};
    return depinject_func(f);
  };
  return lambda_f_to_return;
}

void pause_for_keypress();
void wait_for_any_letter_input(std::istream &is);
void clearScreen();
std::string secondsFormat(double);

#endif
