#include "menu.hpp"
#include "color.hpp"
#include "game.hpp"
#include "scores.hpp"
#include <iostream>
#include <sstream>

int language;

void Menu::selectLanguage(int err){
  constexpr auto greeting_text = "Welcome to ";
  constexpr auto gamename_text = "2048!";
  constexpr auto sp = " ";

  constexpr auto select_text = "Select Your Language";
  constexpr auto eng_text = "1. English";
  constexpr auto kor_text = "2. Korean";

  std::ostringstream str_os1;
  std::ostringstream title_richtext;
  title_richtext << bold_on << sp << greeting_text << blue << gamename_text
                << def << bold_off << "\n\n";

  std::ostringstream language_richtext;
  language_richtext << bold_on << sp << select_text << "\n" 
                    << "   " << blue << eng_text << "  " << blue << kor_text << bold_off << "\n\n";

  clearScreen();
  drawAscii();
  str_os1 << title_richtext.str();
  str_os1 << language_richtext.str();
  std::cout << str_os1.str();

  input_language(err);
}

void Menu::input_language(int err) {
  constexpr auto err_input_text = "Invalid input. Please try again.";
  constexpr auto prompt_choice_text = "Enter Choice: ";
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream err_input_richtext;
  err_input_richtext << red << sp << err_input_text << def << "\n\n";
  std::ostringstream prompt_choice_richtext;
  prompt_choice_richtext << sp << prompt_choice_text;

  if (err) {
    str_os << err_input_richtext.str();
  }

  str_os << prompt_choice_richtext.str();
  std::cout << str_os.str();
  char c;
  std::cin >> c;

  if (std::cin.eof()) {
    std::cout << std::endl;
    exit(EXIT_SUCCESS);
  }

  switch (c) {
  case '1':
    language = 1;
    startMenu(0);
    break;
  case '2':
    language = 2;
    startMenu(0);
    break;
  default:
    selectLanguage(1);
    break;
  }
}


void Menu::startMenu(int err) {
  constexpr auto gamename_text = "2048";
  constexpr auto sp = "  ";
  std::ostringstream str_os;
  std::ostringstream title_richtext;
  if(language == 1){
    constexpr auto greetings_text = "Welcome to ";
    title_richtext << bold_on << sp << greetings_text << blue << gamename_text << "!"
                 << def << bold_off << "\n";
  }
  if(language == 2) {
    constexpr auto greetings_text = "에 오신 것을 환영합니다!";
    title_richtext << bold_on << sp << blue << gamename_text << def << greetings_text 
                  << bold_off << "\n";
  }

  

  

  clearScreen();
  drawAscii();
  if(language == 1){
    constexpr auto menu_entry_text = R"(
          1. Play a New Game
          2. Continue Previous Game
          3. View Highscores and Statistics
          4. Exit

)";
    str_os << title_richtext.str();
    str_os << menu_entry_text;
    std::cout << str_os.str();
  }
  if(language == 2){
    constexpr auto menu_entry_text = R"(
          1. 새 게임
          2. 이어하기
          3. 게임 기록 및 통계
          4. 종료

)";
    str_os << title_richtext.str();
    str_os << menu_entry_text;
    std::cout << str_os.str();
  }
  
  input(err);
}

void Menu::input(int err) {
  
  constexpr auto sp = "  ";

  std::ostringstream str_os;
  std::ostringstream err_input_richtext;
  std::ostringstream prompt_choice_richtext;
  if(language == 1){
    constexpr auto err_input_text = "Invalid input. Please try again.";
    constexpr auto prompt_choice_text = "Enter Choice: ";
    err_input_richtext << red << sp << err_input_text << def << "\n\n";
    prompt_choice_richtext << sp << prompt_choice_text;
  }
  if(language == 2){
    constexpr auto err_input_text = "잘못된 입력입니다. 다시 시도해주세요.";
    constexpr auto prompt_choice_text = "번호를 선택하세요: ";
    err_input_richtext << red << sp << err_input_text << def << "\n\n";
    prompt_choice_richtext << sp << prompt_choice_text;
  }
  

  if (err) {
    str_os << err_input_richtext.str();
  }

  str_os << prompt_choice_richtext.str();
  std::cout << str_os.str();
  char c;
  std::cin >> c;

  if (std::cin.eof()) {
    std::cout << std::endl;
    exit(EXIT_SUCCESS);
  }

  switch (c) {
  case '1':
    startGame();
    break;
  case '2':
    continueGame();
    break;
  case '3':
    showScores();
    break;
  case '4':
    exit(EXIT_SUCCESS);
  default:
    startMenu(1);
    break;
  }
}

void Menu::startGame() {

  Game g;
  g.startGame();
}

void Menu::continueGame() {
  Game g;
  g.continueGame();
}

void Menu::showScores() {

  Scoreboard s;
  s.printScore();
  s.printStats();
}


void drawAscii() {
  constexpr auto title_card_2048 = R"(
   /\\\\\\\\\          /\\\\\\\                /\\\         /\\\\\\\\\
  /\\\///////\\\      /\\\/////\\\            /\\\\\       /\\\///////\\\
  \///      \//\\\    /\\\    \//\\\         /\\\/\\\      \/\\\     \/\\\
             /\\\/    \/\\\     \/\\\       /\\\/\/\\\      \///\\\\\\\\\/
           /\\\//      \/\\\     \/\\\     /\\\/  \/\\\       /\\\///////\\\
         /\\\//         \/\\\     \/\\\   /\\\\\\\\\\\\\\\\   /\\\      \//\\\
        /\\\/            \//\\\    /\\\   \///////////\\\//   \//\\\      /\\\
        /\\\\\\\\\\\\\\\   \///\\\\\\\/              \/\\\      \///\\\\\\\\\/
        \///////////////      \///////                \///         \/////////
  )";
  std::ostringstream title_card_richtext;
  title_card_richtext << green << bold_on << title_card_2048 << bold_off << def;
  title_card_richtext << "\n\n\n";
  std::cout << title_card_richtext.str();
}
