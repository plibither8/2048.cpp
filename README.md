[![Build Status](https://travis-ci.com/fortran-gaming/2048.cpp.svg?branch=master)](https://travis-ci.com/fortran-gaming/2048.cpp)

# 2048.cpp

[![Made with Love in India](https://madewithlove.org.in/badge.svg)](https://madewithlove.org.in/)

> Console version of the game "2048" for GNU/Linux written in C++.
>
> Ability to save scores, choose board size and view statistics!

![Demo of usage](assets/demo.gif)

## To-Do:
- [x] Add start menu [19/04/2018]
- [x] Save highscore / score [21/04/2018]
- [ ] ~~Save a game state and play from a saved game state~~ *(Abandoned indefinitely)*
- [ ] ~~AI~~ *(Abandoned indefinitely)*

## Setup Instructions
The game and code is natively made to run on the GNU/Linux and MacOS platforms, but cross-platform compatibility for Windows has been added too.

### Requirements:
* C++ compiler (e.g. `g++`, `clang++`, etc.)
* Linux, MacOS OR Windows with a working terminal (Cygwin, Windows Subsystem for Linux or Git bash is recommended for Windows)

### Instructions:
1. Open your terminal in your preferred directory and clone this project:
```bash
$ git clone https://github.com/plibither8/2048.cpp
```
2. Enter the project directory:
```bash
$ cd 2048.cpp
```
3. Create a `build` folder (Optional):
```bash
$ mkdir build
```
4. Compile the `2048.cpp` file and create an output using the Makefile:
```bash
$ make
```
5. Run the program and play the game!
```bash
$ ./build/2048.out
```

## Notes:


* Game board array is defined as `board[y][x]`
* Game board will follow the following structure:

```
'x' => x-axis OR the horizotal line OR rows
'y' => y-axis OR the vertical line OR columns
```

### For example (zero-indexed):

`board[2][0]` refers to the 0th tile (or column) in 2nd row as in this case, x = 0 and y = 2. The specific tile is denoted the by '@' symbol in the following gameboard:

```
+------+------+------+------+
|      |      |      |      |
+------+------+------+------+
|      |      |      |      |
+------+------+------+------+
|   @  |      |      |      |
+------+------+------+------+
|      |      |      |      |
+------+------+------+------+
```

# License

Copyright (c) Mihir Chaturvedi. All rights reserved.

Licensed under the [MIT](LICENSE) License.
