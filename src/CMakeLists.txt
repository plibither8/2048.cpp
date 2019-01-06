cmake_minimum_required(VERSION 3.3)  # 3.3: CXX_STANDARD
project(2048 CXX)

set(MAIN_TARGET_NAME 2048)
set(SOURCES 2048.cpp  gameboard.cpp  game.cpp  global.cpp  menu.cpp  scores.cpp  statistics.cpp)

add_executable(${MAIN_TARGET_NAME} ${SOURCES})
target_include_directories(${MAIN_TARGET_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/headers/)

set_target_properties(${MAIN_TARGET_NAME} PROPERTIES
  CXX_STANDARD 11 
  CXX_STANDARD_REQUIRED true)

