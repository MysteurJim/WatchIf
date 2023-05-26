# Variables
CC = gcc
CLIENT_CFLAGS = -Wall -Wextra -O3 -rdynamic -lm -g `pkg-config --cflags gtk+-3.0`
SERVER_CFLAGS = -Wall -Wextra
CLIENT_LDFLAGS = `pkg-config --libs gtk+-3.0`
SERVER_LDFLAGS = -lsqlite3

# Répertoire des fichiers source
SRCDIR = .

# Liste des fichiers source
CLIENT_SOURCE = client.c $(SRCDIR)/queue.c $(SRCDIR)/shared_queue.c
SERVER_SOURCE = server.c $(SRCDIR)/queue.c $(SRCDIR)/shared_queue.c
CLIENT_OBJECTS = $(CLIENT_SOURCE:.c=.o)
SERVER_OBJECTS = $(SERVER_SOURCE:.c=.o)

# Exécutables
CLIENT_EXECUTABLE = client
SERVER_EXECUTABLE = server

# Règles de compilation
all: $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE)

$(CLIENT_EXECUTABLE): $(CLIENT_OBJECTS)
	$(CC) $(CLIENT_CFLAGS) $(CLIENT_OBJECTS) $(CLIENT_LDFLAGS) -o $@

$(SERVER_EXECUTABLE): $(SERVER_OBJECTS)
	$(CC) $(SERVER_CFLAGS) $(SERVER_OBJECTS) $(SERVER_LDFLAGS) -o $@

# Compilation des fichiers source en fichiers objet
$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CLIENT_CFLAGS) $(SERVER_CFLAGS) -c $< -o $@

# Nettoyage des fichiers objets et des exécutables
clean:
	rm -f $(CLIENT_OBJECTS) $(SERVER_OBJECTS) $(CLIENT_EXECUTABLE) $(SERVER_EXECUTABLE)

