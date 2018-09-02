cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(2048 CXX)

add_executable(2048 
  global.cpp 
  game.cpp 
  menu.cpp 
  scores.cpp 
  statistics.cpp 
  2048.cpp
)

target_compile_features(2048 
  PRIVATE 
    cxx_std_11
)
