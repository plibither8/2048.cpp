# 2048.cpp

#### Console version of the game "2048" for GNU/Linux written in C++

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