# 2048.cpp

[![Build Status](https://travis-ci.org/plibither8/2048.cpp.svg?branch=master)](https://travis-ci.org/plibither8/2048.cpp)
[![Build status](https://ci.appveyor.com/api/projects/status/sljhlvcx8k11ckw7?svg=true)](https://ci.appveyor.com/project/plibither8/2048-cpp)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/b37414d66e7d4146bf72a4a467fdc84d)](https://app.codacy.com/app/plibither8/2048.cpp?utm_source=github.com&utm_medium=referral&utm_content=plibither8/2048.cpp&utm_campaign=Badge_Grade_Dashboard)
[![Language grade: C/C++](https://img.shields.io/lgtm/grade/cpp/g/plibither8/2048.cpp.svg?logo=lgtm&logoWidth=18)](https://lgtm.com/projects/g/plibither8/2048.cpp/context:cpp)
[![Made with Love in India](https://madewithlove.org.in/badge.svg)](https://madewithlove.org.in/)
[![Run on Repl.it](https://repl.it/badge/github/plibither8/2048.cpp)](https://repl.it/github/plibither8/2048.cpp)

> C++ 로 쓰여진 2048 게임의 최종 버전입니다.

:tada: GitHub의 [Twitter](https://twitter.com/github/status/1017094930991370240) 와 [Facebook](https://www.facebook.com/GitHub/videos/1433491453419807/) 페이지에 소개되어 있습니다! :tada:

<p align="center">
    <img align="center" alt="2048 in action!" src="assets/demo.gif"></img>
</p>

## 호환성

게임과 코드는 GNU/리눅스 및 MacOS 플랫폼에서 기본적으로 실행되도록 만들어졌지만, 윈도우를 위한 크로스 플랫폼 호환성도 추가했습니다.

### 요구사항

* C++11 컴파일러 (e.g. `g++`, `clang++`, `pgc++`, `icpc`, etc.)
* Virtually any platform including:
  * Linux
  * MacOS
  * Windows (via Cygwin or Windows Subsystem for Linux)
* [CMake](https://cmake.org/) or [Meson](https://mesonbuild.com/)

### 설치방법

1. 원하는 디렉토리에서 터미널을 열고 이 프로젝트를 복제하십시오:
```sh
git clone https://github.com/plibither8/2048.cpp
```
2. 프로젝트의 빌드 디렉토리를 입력하십시오:
```sh
cd 2048.cpp/build
```

---

CMake와 Meson 모두에 대해, 시스템의 기본 C++ 컴파일러가 사용됩니다.
C++ 컴파일러를 수동으로 선택하려면 CXX=clang++ cmake 또는 CXX=clang++ meson 등을 선택적으로 추가하십시오.

#### CMake로 빌드하기

3. 빌드 구성을 생성하십시오.
```sh
cmake ../
```
4. 실행 파일을 빌드하십시오.
```sh
cmake --build .
```
5. 프로그램을 설치하십시오. (옵션)
```sh
cmake --build . --target install
```

6. 프로그램을 실행해서 게임을 하십시오! :tada:
```sh
2048    # run `./2048` if game is not installed
```

<p align="center">
    <b>OR</b>
</p>

#### Meson으로 빌드하기

3. 빌드 구성을 생성하십시오.
```sh
meson ../
```
4. Build the executable
```sh
ninja
```
5. 프로그램을 설치하십시오. (옵션)
```sh
meson configure --prefix=$HOME/.local
ninja install
```

6. 프로그램을 실행해서 게임을 하십시오! :tada:
```sh
2048    # run `./2048` if game is not installed
```

## 기여활동

먼저, 여러분들의 기여에 감사드립니다! 몇가지 공지할 것이 있습니다:

* 버그를 발견했거나 구현하고 싶은 기능이 있는 경우 문제를 제기하십시오, [raise an issue](https://github.com/plibither8/2048.cpp/issues).

* pull 요청을 제안한 경우, 원본 코드(.cpp 및 .hpp) 파일에서 `clang 형식`을 실행하십시오.

* 로컬 저장소(리포지토리)에서 `git update-index --skip-worktree ./data/*.txt`를 실행하여 데이터 파일의 변경 내용이 git에 의해 추적되지 않도록 하십시오.

## 감사

다음과 같은 사람들의 도움에 깊이 감사드립니다:

* [Michael Hirsch](https://github.com/scivision)
  * cleaned up the code,
  * organised the header files in a better way for a more efficient build,
  * added the AppVeyor CI,
  * added the Meson build system and, fixed CMake and added install feature.
* [Aiman Ismail](https://github.com/pokgak) added support for Vim keybinding.
* [Patrik Huber](https://github.com/patrikhuber) fixed a typo in the Readme.
* [zestze](https://github.com/zestze) changed `cstdlib rand` to C++ random int generator.
* [Pascal J. Bourguignon](https://github.com/informatimago) added support for ANSI arrow keys.
* [Jean-Michaël Celerier](https://github.com/jcelerier) added `CMakeLists.txt` file.
* [comwrg](https://github.com/comwrg) made the duration in the statistics and highscores human-readable, wrapping seconds to minutes and hours.
* [Christian Bundy](https://github.com/christianbundy) replaced the ugly -, + and | with box-drawing characters.
* [Tor E Hagemann](https://github.com/hagemt) fixed issue #10, causing unwanted character `1` to be printed.
* [farazxameer](https://github.com/farazxameer) implemented feature to save a game state and continue from a saved game state, refined game logic.
* [drodil](https://github.com/drodil) implemented checks to ascertain existence of data files, fixed issue #12
* [Aviskar KC](https://github.com/aviskarkc10) added arrow keys to game instructions.
* [Peter Squicciarini](https://github.com/stripedpajamas) fixed readme instructions.
* [Mark Fischer, Jr.](https://github.com/flyingfisch) fixed a typo.
* [Hugo Brandão](https://github.com/jhugobb) completely reorganised the existing project structure and updated the build method to enfore CMake.
* [Alton Alvarez](https://github.com/mathematicalist) fixed a typo.
* [cawvyoct](https://github.com/cawvyoct) made the source code much, much easier to read: removed most magic numbers, replacing them with variables to ease maintenance. Implemented `clang-format`.
* [Cong](https://github.com/cxong) edited the `Game` class constructor.
* [Tien Do](https://github.com/tiendq) added the 'exit' option in the main menu, updated the `CMakeLists.txt` file and made `Color` enum to a scoped enum.
* [ScorrMorr](https://github.com/ScorrMorr) made many methods `const`s.
* [tangmengqiu](https://github.com/tangmengqiu) fixed an error-causing instruction step in the readme.

### 관리자

* [Mihir Chaturvedi](https://github.com/plibither8)
* [cawvyoct](https://github.com/cawvyoct)

## 참고

`[GameBoard].getTile(2,0)` 는 x = 0, y = 2 처럼 2행의 0번째 타일(또는 열)을 가리킨다. 다음 게임보드에서 특정 타일은 '@' 기호로 표시됩니다:

> 참고: 행 및 열 인덱싱은 0부터 시작합니다.

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

## 개발예정

- [x] 시작 메뉴 추가 [19/04/2018]
- [x] 고득점/점수 저장 [21/04/2018]
- [x] 게임 상태를 저장하고 저장된 게임 상태에서 재생
- [ ] ~~AI~~ *(Abandoned indefinitely)*

---

## 라이선스

Copyright (c) Mihir Chaturvedi. All rights reserved.

Licensed under the [MIT](LICENSE) License.
