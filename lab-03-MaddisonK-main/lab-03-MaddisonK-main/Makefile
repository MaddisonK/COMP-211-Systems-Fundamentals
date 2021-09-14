CC=gcc
CFLAGS=-c -Wall -Werror -g

all: lab03

lab03: sort.o lab03.o
	$(CC) sort.o lab03.o -o lab03 -lm

sort.o: sort.c
	$(CC) $(CFLAGS) sort.c

lab03.o: lab03.c
	$(CC) $(CFLAGS) lab03.c

clean:
	/bin/rm -f lab03 *.o *.gz

