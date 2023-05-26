#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <err.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sqlite3.h>
#include "shared_queue.h"
#include <pthread.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 50
#define THREAD_COUNT 4

typedef struct
{
    char id[128];
    char username[128];
    char password[128];
    char email[128];
}Infos;


static int init_server();
static int new_connextion(int socketServer);
static void write_client_int(int sock,int mess);
void sign_in(int sock);
int read_client_info(int sock,Infos* info);
int read_db_Users(const char *buffer,Infos* c);
#endif
