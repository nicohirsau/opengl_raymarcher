CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -std=c++17 -static -static-libgcc -static-libstdc++ 

BIN		:= bin
SRC		:= src
INCLUDE	:= -Iinclude -Iexternal/include
LIB		:= -Lexternal/lib

LIBRARIES	:= -lglfw3 -lgdi32 -lglad

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

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp $(SRC)/Mantaray/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $(LIB) $^ -o $@ $(LIBRARIES)
