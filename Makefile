#The main 
# FILES = atomic.cpp atom.cpp test.cpp 
FILES = atomic.cpp atom.cpp term_disp.cpp game_step.cpp

#CC specifies which compiler we're using
# CC = g++
CC = clang 

#COMPILER_FLAGS specifies the additional compilation options we're using
COMPILER_FLAGS = -g -Wno-write-strings
# COMPILER_FLAGS = -g -Wno-write-strings -Wno-conversion-null

#LINKER_FLAGS specifies the libraries we're linking against
# LINKER_FLAGS = -lm -lsfml-graphics -lsfml-window -lsfml-system -lGL -lGLU

#specifies the name of our exectuable
PROJ_NAME = atomic


#compile all files
all :
	clear && clear && $(CC) -o $(PROJ_NAME) $(FILES) $(COMPILER_FLAGS) $(LINKER_FLAGS) 
