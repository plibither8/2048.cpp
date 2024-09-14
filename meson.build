project('2048', 'cpp',
  default_options : ['cpp_std=c++17', 'warning_level=2'])

main_target_name = '2048'

sources = ['src/2048.cpp', 'src/gameboard.cpp', 'src/gameboard-graphics.cpp', 'src/game.cpp', 'src/game-input.cpp', 'src/game-graphics.cpp', 'src/game-pregamemenu.cpp', 'src/global.cpp', 'src/loadresource.cpp', 'src/menu.cpp', 'src/menu-graphics.cpp', 'src/saveresource.cpp', 'src/scores.cpp', 'src/scores-graphics.cpp', 'src/statistics.cpp', 'src/statistics-graphics.cpp', 'src/tile.cpp', 'src/tile-graphics.cpp']
hdrs = include_directories('src/headers')

executable(main_target_name, sources,
  include_directories : hdrs,
  install : true)

install_data(['data/scores.txt', 'data/statistics.txt'],
  install_dir : 'data')

