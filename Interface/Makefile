CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)

SRC = main.c

OBJS = $(SRC:.c=.o)

all: main

%.o: %.c
	$(CC) -c -o $(@F) $(CFLAGS) $<

main: $(OBJS)
	$(CC) -o $(@F) $(LIBS) $(OBJS)

.PHONY: clean

clean:
	rm -f $(OBJS)
