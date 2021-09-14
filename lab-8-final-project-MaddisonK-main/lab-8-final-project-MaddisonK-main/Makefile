CC=gcc
CFLAGS=-c -Wall -g

all: driver

driver: shell.o driver.o
	$(CC) shell.o driver.o -o driver

shell.o: shell.c
	$(CC) $(CFLAGS) shell.c

driver.o: driver.c
	$(CC) $(CFLAGS) driver.c

clean:
	/bin/rm -f driver *.o *.gz

run:
	./driver

tarball:
	# put your tar command here
	# tar -cvzf <lastname>.tar.gz *

