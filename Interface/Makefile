CC ?= gcc
PKGCONFIG = $(shell which pkg-config)
CFLAGS = $(shell $(PKGCONFIG) --cflags gtk+-3.0)
LIBS = $(shell $(PKGCONFIG) --libs gtk+-3.0)

SRC = main.c Network/client.c Network/queue.c Network/shared_queue.c
OBJDIR = obj

OBJS = $(addprefix $(OBJDIR)/,$(SRC:.c=.o))

all: main

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c -o $@ $(CFLAGS) $<

main: $(OBJS)
	$(CC) -o $@ $(LIBS) $^

.PHONY: clean

clean:
	rm -rf $(OBJDIR)
	rm -f main
	rm -f main.o
