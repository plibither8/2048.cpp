# 2048.cpp

[![Build Status](https://img.shields.io/travis/plibither8/2048.cpp.svg)](https://travis-ci.com/plibither8/2048.cpp)
[![Build status](https://ci.appveyor.com/api/projects/status/sljhlvcx8k11ckw7?svg=true)](https://ci.appveyor.com/project/plibither8/2048-cpp)
[![Made with Love in India](https://madewithlove.org.in/badge.svg)](https://madewithlove.org.in/)

> Terminal version of the game "2048" written in C++.

:tada: Featured on GitHub's [Twitter](https://twitter.com/github/status/1017094930991370240) and [Facebook](https://www.facebook.com/GitHub/videos/1433491453419807/) pages! :tada:

![Demo of usage](assets/demo.gif)

## To-Do

- [x] Add start menu [19/04/2018]
- [x] Save highscore / score [21/04/2018]
- [x] Save a game state and play from a saved game state
- [ ] ~~AI~~ *(Abandoned indefinitely)*

## Setup

The game and code is natively made to run on the GNU/Linux and MacOS platforms, but cross-platform compatibility for Windows has been added too.

### Requirements

* C++ compiler (e.g. `g++`, `clang++`, etc.)
* Linux, MacOS OR Windows with a working terminal (Cygwin, Windows Subsystem for Linux or Git bash is recommended for Windows)
* [CMake](https://cmake.org/)

### Installation

1. Open your terminal in your preferred directory and clone this project:
```bash
$ git clone https://github.com/plibither8/2048.cpp
```
2. Enter the project directory:
```bash
$ cd 2048.cpp
```
3. Create and enter the build directory:
```bash
$ mkdir build && cd build
```
4. Compile the program with cmake
```bash
$ cmake ..
```
5. Build the executable
```bash
$ make
```
6. Run the program and play the game!
```bash
$ ./2048.out
```

## Notes


* Game board array is defined as `board[y][x]`
* Game board will follow the following structure:

```
'x' => x-axis OR the horizontal line OR columns
'y' => y-axis OR the vertical line OR rows
```

### For example (zero-indexed)

`board[2][0]` refers to the 0th tile (or column) in 2nd row as in this case, x = 0 and y = 2. The specific tile is denoted the by '@' symbol in the following gameboard:

```
┌──────┬──────┬──────┬──────┐
│      │      │      │      │
├──────┼──────┼──────┼──────┤
│      │      │      │      │
├──────┼──────┼──────┼──────┤
│   @  │      │      │      │
├──────┼──────┼──────┼──────┤
│      │      │      │      │
└──────┴──────┴──────┴──────┘
```

## License

Copyright (c) Mihir Chaturvedi. All rights reserved.

Licensed under the [MIT](LICENSE) License.
