CC = gcc
CFLAGS=-g -Wall
OBJS = main.o coroutine.o cubefunc.o cubeapp.o
TARGET = runcube

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

coroutine.o : coroutine.h coroutine.c
	$(CC) -c coroutine.c

cubefunc.o : coroutine.h cube.h coroutine.o cubefunc.c
	$(CC) -c cubefunc.c

cubeapp.o : coroutine.h cube.h coroutine.o cubefunc.c cubeapp.c
	$(CC) -c cubeapp.c

main.o : coroutine.h cube.h coroutine.o cubefunc.c cubeapp.c main.c
	$(CC) -c main.c

.PHONY: clean
clean:
	rm -f $(OBJS) main