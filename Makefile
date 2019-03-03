CC=g++
CFLAGS_COMPILE=-c -Wall -Wextra -pedantic
CFLAGS_LINK=-lsfml-graphics -lsfml-window -lsfml-system -lsfml-newtwork
OBJ=main.o engine.o net.o graphics.o
PROG=game

all: $(PROG)

$(PROG): $(OBJ)
	$(CC) -o $(PROG) $(OBJ) $(CFLAGS_LINK)

.cpp.o:
  $(CC) $(CFLAGS_COMPILE) $<

clean:
  $(RM) $(OBJ)
