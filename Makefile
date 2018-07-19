
#SRC specifies which files to compile as part of the project
SRC = 2048.cpp
EXE = build/2048.out
#CXX specifies C++ compiler
#CXX = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
CXXFLAGS = -std=c++11 #-Wall
#  CXXFLAGS += -w


#This is the target that compiles our executable
$(EXE): $(SRC)
	$(CXX) $^ $(CXXFLAGS) -o $@
		
clean:
	$(RM) *.o $(EXE)
