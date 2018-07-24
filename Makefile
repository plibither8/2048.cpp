
#SRC specifies which files to compile as part of the project
EXE = build/2048.out
#CXX specifies C++ compiler
#CXX = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
CXXFLAGS = -std=c++11 #-Wall
#  CXXFLAGS += -w

#This is the target that compiles our executable
$(EXE): global.o game.o menu.o scores.o statistics.o 2048.cpp
	$(CXX) $^ $(CXXFLAGS) -o $@


global.o: global.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

game.o: game.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

menu.o: menu.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

scores.o: scores.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

statistics.o: statistics.cpp
	$(CXX) -c $^ $(CXXFLAGS) -o $@

test:
	echo "" | ./$(EXE)

clean:
	$(RM) *.o $(EXE)
