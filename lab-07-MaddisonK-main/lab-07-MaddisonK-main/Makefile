CC=gcc
CFLAGS=-c -Wall -g 

all: lab07

lab07: alu.o lab07.o
	$(CC) alu.o lab07.o -o lab07 -lm

alu.o: alu.c
	$(CC) $(CFLAGS) alu.c

lab07.o: lab07.c
	$(CC) $(CFLAGS) lab07.c

clean:
	/bin/rm -f lab07 *.o *.gz

run:
	./lab07
