CC=gcc
CFLAGS=-Wall -Werror -pedantic -g -std=c11
OBJ=main.c lib.h lib.c
NAME=pytagoGlob
LIBS= -lm -lgmp -lmpfr
DEBUG= -D DEBUG
MIRROW= -D MIRROW

#gcc -o collision3 collison3.c -lgmp -lmpfr -lcrypto  -lpthread -lm
#gcc -o calcFromPrivate calcFromPrivate.c -lgmp -lcrypto -lm
#gcc -o collision collision.c -lgmp -lmpfr -lcrypto  -lpthread -lm -Wall -Werror -g
#gcc -o collision0 collison.c -lgmp -lmpfr -lcrypto  -lpthread -lm
#gcc -o collision1 collisonS.c -lgmp -lmpfr -lcrypto  -lpthread -lm
#gcc -o collision2 collison2.c -lgmp -lmpfr -lcrypto  -lpthread -lm

default:
	$(CC) -o $(NAME) $(OBJ) $(LIBS) $(CFLAGS)
debug:
	$(CC) $(DEBUG) -o $(NAME) $(OBJ) $(LIBS) $(CFLAGS)
mirrow:
	$(CC) $(MIRROW) -o $(NAME) $(OBJ) $(LIBS) $(CFLAGS)
mirrowDebug:
	$(CC) $(DEBUG) $(MIRROW) -o $(NAME) $(OBJ) $(LIBS) $(CFLAGS)
clean:
	rm -f $(NAME)
