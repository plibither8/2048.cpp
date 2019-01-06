project('2048', 'cpp', 
  default_options : ['cpp_std=c++11'])

main_target_name = '2048'
sources = ['2048.cpp', 'gameboard.cpp', 'game.cpp', 'global.cpp', 'menu.cpp', 'scores.cpp', 'statistics.cpp']
hdrs = include_directories('headers')

MAIN_TARGET_NAME_exe = executable(main_target_name, sources, include_directories : hdrs)

