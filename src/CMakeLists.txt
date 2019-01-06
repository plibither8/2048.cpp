cmake_minimum_required(VERSION 3.7)  # 3.7: CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT, 3.3: CXX_STANDARD
project(2048 CXX)

set(SOURCES 2048.cpp  gameboard.cpp  game.cpp  global.cpp  menu.cpp  scores.cpp  statistics.cpp)

add_executable(2048 ${SOURCES})
target_include_directories(2048 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/headers/)

set_target_properties(2048 PROPERTIES
  CXX_STANDARD 11 
  CXX_STANDARD_REQUIRED true)

# --- install

if(UNIX AND CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "~/.local" CACHE PATH "..." FORCE)
endif()

install(TARGETS 2048
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../data
        DESTINATION ${CMAKE_INSTALL_PREFIX})
