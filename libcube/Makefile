CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LDFLAGS = -shared
LIBRARY = libcube.so
SOURCES = coroutine.c cubefunc.c cubeio.c
OBJECTS = $(SOURCES:.c=.o)

all: $(LIBRARY)

$(LIBRARY): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $(LIBRARY) $(OBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS)