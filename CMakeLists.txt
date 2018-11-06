cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(2048 CXX)

set(MAIN_TARGET_NAME 2048)
set(CMAKE_CXX_STANDARD 11)
# Source Files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"src/*.cpp"
)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
	"src/headers/*.hpp"
)

include_directories("src/headers")
add_executable(${MAIN_TARGET_NAME} ${SOURCES} ${HEADERS})
# target_compile_features(${MAIN_TARGET_NAME} PRIVATE cxx_std_11)
# target_include_directories(${MAIN_TARGET_NAME} PRIVATE "src/headers/")
