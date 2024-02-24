CC=gcc
CFLAGS=-I.

my_fm: my_fm.o
	$(CC) -o my_fm my_fm.o

my_fm.o: my_fm.c
	$(CC) -c my_fm.c

clean:
	rm -f my_fm my_fm.o
