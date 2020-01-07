CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -std=c++11 -static -static-libgcc -static-libstdc++ 

BIN		:= bin
SRC		:= src
INCLUDE	:= -iquote include -I external/include
LIB		:= -L external/lib -L lib

LIBRARIES	:= -lglfw3 -lgdi32 -lglad -lmantaray

EXECUTABLE_NAME := build
ifeq ($(OS),Windows_NT)
EXECUTABLE	:= $(EXECUTABLE_NAME).exe
else
EXECUTABLE	:= $(EXECUTABLE_NAME)
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $(LIB) $^ -o $@ $(LIBRARIES)
