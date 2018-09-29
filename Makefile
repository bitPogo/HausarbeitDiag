CC=gcc
CFLAGS=--std=c99 -Wall -Werror -pedantic -g
OBJ=main.c lib.h lib.c
NAME=ptMap
LIBS= -lm -lgmp -lmpfr -lpthread

default:
	$(CC) -o $(NAME) $(OBJ) $(LIBS) $(CFLAGS)
clean:
	rm -f $(NAME)
