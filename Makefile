CXX		  := g++
CXX_FLAGS := -Wall -Wextra -std=c++11 -ggdb -w -static

BIN		:= bin
SRC		:= src
INCLUDE	:= include -I C:\SFML-2.5.1\include -DSFML_STATIC
LIB		:= lib -L C:\mySFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lgdi32 -lwinmm -lopengl32 -lfreetype

LIBRARIES	:= C:\mySFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -static-libgcc -static-libstdc++
EXECUTABLE	:= main


all: $(BIN)/$(EXECUTABLE)

run: clean all
	clear
	./$(BIN)/$(EXECUTABLE)

$(BIN)/$(EXECUTABLE): $(SRC)/*.cpp
	$(CXX) $^ $(CXX_FLAGS)  -I$(INCLUDE) -L$(LIB)  -o $@

clean:
	-rm $(BIN)/*.
# g++ -Wall -Wextra -std=c++17 -ggdb -Iinclude -IC:\SFML-2.5.1\include -Llib -LC:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -o bin/main C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system

# g++ src\main.cpp  -Wall -Wextra -std=c++17 -ggdb -Iinclude -IC:\SFML-2.5.1\include -Llib -LC:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system -o bin/main 
# g++ src/main.cpp -Wall -Wextra -std=c++17 -ggdb -Wl,-subsystem,windows -Iinclude -I C:\SFML-2.5.1\include -Llib -L C:\SFML-2.5.1\lib -lsfml-graphics -lsfml-window -lsfml-system  -o bin/main
#-I C:\Users\Retsal\Documents\Cpp\SfmlTest\include -DSFML_STATIC -L C:\Users\Retsal\Documents\Cpp\SfmlTest\lib -l sfml-graphics-s -l sfml-window-s -l sfml-system-s  -l gdi32 -l winmm -l opengl32 -l freetype
# -Iinclude -I C:\SFML-2.5.1\include -DSFML_STATIC -Llib -L C:\mySFML\lib -lsfml-graphics-s -lsfml-window-s -lsfml-system-s  -o bin/main