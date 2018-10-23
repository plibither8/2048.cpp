# 2048.cpp

<!--[![Build Status](https://img.shields.io/travis/plibither8/2048.cpp.svg)](https://travis-ci.com/plibither8/2048.cpp)-->
[![Build status](https://ci.appveyor.com/api/projects/status/sljhlvcx8k11ckw7?svg=true)](https://ci.appveyor.com/project/plibither8/2048-cpp)
[![Made with Love in India](https://madewithlove.org.in/badge.svg)](https://madewithlove.org.in/)

> Terminal version of the game "2048" written in C++.

:tada: Featured on GitHub's [Twitter](https://twitter.com/github/status/1017094930991370240) and [Facebook](https://www.facebook.com/GitHub/videos/1433491453419807/) pages! :tada:

![Demo of usage](assets/demo.gif)

## Setup

The game and code is made to run natively on the GNU/Linux and macOS platforms, but cross-platform compatibility for Windows has been added too.

### Requirements

* C++ compiler (e.g. `g++`, `clang++`, etc.)
* Linux, macOS OR Windows with a working terminal (Cygwin, Windows Subsystem for Linux or Git bash is recommended for Windows)
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
6. Run the program and play the game! :tada:
```bash
$ ./2048
```

## Contributing

First of all, thank you for contributing :smile:! A few things to note:

* If you have found a bug, or have a feature that you'd like implemeted, [raise an issue](https://github.com/plibither8/2048.cpp/issues).

* If you have proposed a pull request, make sure that you run `clang-format` on the source code (both, `.cpp` and `.hpp`) files if you've made changes there.

## Thanks

I deeply appreciate the help of the following people:

* [Michael Hirsch](https://github.com/scivision) cleaned up the code, organised the header files in a better way for a more efficient build and added the AppVeyor CI.
* [Aiman Ismail](https://github.com/pokgak) added support for Vim keybinding.
* [Patrik Huber](https://github.com/patrikhuber) fixed a typo in the Readme.
* [zestze](https://github.com/zestze) changed `cstdlib rand` to C++ random int generator.
* [Pascal J. Bourguignon](https://github.com/informatimago) added support for ANSI arrow keys.
* [Jean-Michaël Celerier](https://github.com/jcelerier) added `CMakeLists.txt` file.
* [comwrg](https://github.com/comwrg) made the duration in the statistics and highscores human-readable, wrapping seconds to minutes and hours.
* [Christian Bundy](https://github.com/christianbundy) changed the ugly -, + and | to box-drawing characters.
* [Tor E Hagemann](https://github.com/hagemt) fixed issue #10, causing unwanted character `1` to be printed.
* [farazxameer](https://github.com/farazxameer) implemented feature to save a game state and continue from a saved game state, refined game logic.
* [drodil](https://github.com/drodil) implemented checks to ascertain existance of data files, fixed issue #12
* [Aviskar KC](https://github.com/aviskarkc10) added arrow keys to game instructions.
* [Peter Squicciarini](https://github.com/stripedpajamas) fixed readme instructions.
* [Mark Fischer, Jr.](https://github.com/flyingfisch) fixed a typo.
* [cawvyoct](https://github.com/cawvyoct) made source code much, much easier to read, removing most magic numbers and replacing them with variable to ease maintanence and implemented `clang-format`.

## Notes

* Game board array is defined as `board[y][x]`
* Game board will follow the following structure:

```plaintext
'x' => x-axis OR the horizontal line OR columns
'y' => y-axis OR the vertical line OR rows
```

### Example

`board[2][0]` refers to the 0th tile (or column) in 2nd row as in this case, x = 0 and y = 2. The specific tile is denoted the by '@' symbol in the following gameboard:

> Note: `row` and `column` indexing starts at 0.

```plaintext
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

## To-Do

- [x] Add start menu [19/04/2018]
- [x] Save highscore / score [21/04/2018]
- [x] Save a game state and play from a saved game state
- [ ] ~~AI~~ *(Abandoned indefinitely)*

---

## License

Copyright (c) Mihir Chaturvedi. All rights reserved.

Licensed under the [MIT](LICENSE) License.
