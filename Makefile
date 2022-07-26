CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++17 -ggdb -w

BIN		:= bin
SRC		:= src
INCLUDE	:= include -I C:\SFML-2.5.1\include
LIB		:= lib -L C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system

LIBRARIES	:= C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $^ $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB)  -o $@

clean:
	-rm $(BIN)/*
# g++ -Wall -Wextra -std=c++17 -ggdb -Iinclude -IC:\SFML-2.5.1\include -Llib -LC:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -o bin/main C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system

# g++ src\main.cpp  -Wall -Wextra -std=c++17 -ggdb -Iinclude -IC:\SFML-2.5.1\include -Llib -LC:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -o bin/main 
# g++ src/main.cpp -Wall -Wextra -std=c++17 -ggdb -Wl,-subsystem,windows -Iinclude -I C:\SFML-2.5.1\include -Llib -L C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system  -o bin/main