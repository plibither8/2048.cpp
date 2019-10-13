cmake_minimum_required(VERSION 3.7)  # 3.7: CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT, 3.3: CXX_STANDARD
project(2048 CXX)

set(SOURCES src/2048.cpp src/gameboard.cpp src/gameboard-graphics.cpp src/game.cpp src/game-graphics.cpp src/game-input.cpp src/game-pregamemenu.cpp src/global.cpp src/loadresource.cpp src/menu.cpp src/menu-graphics.cpp src/saveresource.cpp src/scores.cpp src/scores-graphics.cpp src/statistics.cpp src/statistics-graphics.cpp src/tile.cpp src/tile-graphics.cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL GNU)
  list(APPEND FLAGS -Wall)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Clang)
  list(APPEND FLAGS -Wall)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL Intel)
  list(APPEND FLAGS -w2)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL PGI)
  list(APPEND FLAGS -a)
endif()

add_executable(2048 ${SOURCES})
target_include_directories(2048 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/headers/)
target_compile_options(2048 PRIVATE ${FLAGS})

set_target_properties(2048 PROPERTIES
  CXX_STANDARD 14
  CXX_STANDARD_REQUIRED true)

# --- install

if(UNIX AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "~/.local" CACHE PATH "..." FORCE)
endif()

install(TARGETS 2048
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data
        DESTINATION ${CMAKE_INSTALL_PREFIX})
