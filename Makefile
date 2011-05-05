CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=gnu90 -g

all: resolve

clean:
	-rm -f resolve *.o

.PHONY: all clean

