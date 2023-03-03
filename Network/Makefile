CC=gcc
CFLAGS=-Wall -Wextra -pedantic -std=c99 -O2 -I.
LDFLAGS=-lsqlite3

.PHONY: all clean

all: client server

client: client.o
	$(CC) $(LDFLAGS) $^ -o $@

server: server.o
	$(CC) $(LDFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f client server *.o