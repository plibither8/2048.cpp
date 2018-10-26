cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(2048 CXX)

set(MAIN_TARGET_NAME 2048)

# Source Files
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
	"src/*.cpp"
)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
	"src/headers/*.hpp"
)

add_executable(${MAIN_TARGET_NAME} ${SOURCES} ${HEADERS})

target_compile_features(${MAIN_TARGET_NAME} PRIVATE cxx_std_11)

target_include_directories(${MAIN_TARGET_NAME} PRIVATE "src/headers/")
