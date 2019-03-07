CC=g++
CFLAGS_COMPILE=-c -Wall -Wextra -pedantic
CFLAGS_LINK=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-network
OBJ=main.o NetWork.o
CODE=main.cpp NetWork.cpp
PROG=game

all:
	g++ -c -Wall -Wextra main.cpp NetWork.cpp
	g++ -lsfml-graphics -lsfml-window -lsfml-system -lsfml-network main.o NetWork.o -o game

clean:
	rm *.o


