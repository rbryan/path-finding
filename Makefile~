
CC=gcc
CFLAGS=-g -O3 -march=core2 -mtune=core2 -Wall
LFLAGS= -lImlib2 -lm -lpthread

path: path.h path.c Makefile
	$(CC) $(CFLAGS) path.c -o path $(LFLAGS)

clean:
	rm path 
