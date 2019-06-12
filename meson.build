project('2048', 'cpp', 
  default_options : ['cpp_std=c++14'])


cxx = meson.get_compiler('cpp')
if cxx.get_id() in ['gcc', 'clang']
  add_global_arguments('-Wall', language : 'cpp')
elif cxx.get_id() == 'intel'
  add_global_arguments('-w2', language : 'cpp')
endif

main_target_name = '2048'
sources = ['src/2048.cpp', 'src/gameboard.cpp', 'src/game.cpp', 'src/game-input.cpp', 'src/game-graphics.cpp', 'src/game-pregamemenu.cpp', 'src/global.cpp', 'src/loadresource.cpp', 'src/menu.cpp', 'src/saveresource.cpp', 'src/scores.cpp', 'src/statistics.cpp', 'src/tile.cpp']
hdrs = include_directories('src/headers')

executable(main_target_name, sources, 
  include_directories : hdrs,
  install : true)
  
install_data(['data/scores.txt', 'data/statistics.txt'],
  install_dir : 'data')

