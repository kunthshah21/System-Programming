CC=gcc
CFLAGS=-Wall -Wextra -g

all: FLAPS

FLAPS: FLAPS.o
	$(CC) $(CFLAGS) -o FLAPS FLAPS.o

FLAPS.o: FLAPS.c
	$(CC) $(CFLAGS) -c FLAPS.c

clean:
	rm -f FLAPS FLAPS.o
