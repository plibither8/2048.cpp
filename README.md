# 2048.cpp

#### Console version of the game "2048" for GNU/Linux written in C++

## To-Do:
- [x] Add start menu [19/04/2018]
- [x] Save highscore / score [21/04/2018]
- [ ] Save a game state and play from a saved game state
- [ ] AI

## Setup Instructions
The game and code is natively made to run on the GNU/Linux platform, but cross-platform compatibility for Windows has been added too.

### Requirements:
* g++ or gcc compiler
* Linux OR Windows with a working terminal (Cygwin or Git bash for Windows is recommended)

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
4. Compile the `2048.cpp` file and create an output:
```bash
$ g++ --std=c++11 2048.cpp -o ./build/2048.out
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