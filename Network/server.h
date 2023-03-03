#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 50

typedef struct
{
    int sock;
    char name[BUF_SIZE];
}Client;

static int init_server();
static int new_connextion(int socketServer);
static int read_client(int sock,char *buffer);
static void remove_client(Client *clients, int to_remove, int *actual);

#endif