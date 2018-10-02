cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(2048 CXX)

# Source Files
file(GLOB_RECURSE SOURCES 
	"src/*.cpp"
	)
file(GLOB_RECURSE HEADERS
	"src/headers/*.hpp"
	)

include_directories(
	"src/headers/"
)

add_executable(2048 ${SOURCES} ${HEADERS})

target_compile_features(2048 
  PRIVATE 
    cxx_std_11
)
