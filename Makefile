CC		:= g++
D_FLAGS := -g -Wall -Wextra
C_FLAGS := -std=c++11

BIN		:= bin
SRC		:= src
INCLUDE	:= -iquote include -I external/include

EXECUTABLE_NAME := build
ifeq ($(OS),Windows_NT)
EXECUTABLE	:= $(EXECUTABLE_NAME).exe
LIB		:= -L external/lib/windows
LIBRARIES	:= -lmantaray -lglfw3 -lgdi32
else
EXECUTABLE	:= $(EXECUTABLE_NAME)
LIB		:= -L external/lib/linux
LIBRARIES	:= -lmantaray -lglfw3 -lrt -lm -ldl -lX11 -lpthread -lxcb -lXau -lXdmcp
endif

all: $(BIN)/$(EXECUTABLE)

clean:
	$(RM) $(BIN)/$(EXECUTABLE)

run: all
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CC) $(D_FLAGS) $(C_FLAGS) $(INCLUDE) $^ $(LIB) $(LIBRARIES) -o $@
