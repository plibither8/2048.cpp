
#OBJS specifies which files to compile as part of the project
OBJS = 2048.cpp

#CC specifies which compiler we're using
CC = g++

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
#  COMPILER_FLAGS = -w
#
#  #LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = --std=c++11

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ./build/2048.out

#This is the target that compiles our executable
all : $(OBJS)
		$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)