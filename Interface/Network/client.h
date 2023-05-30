#ifndef CLIENT_H
#define CLIENT_H

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
#include <fcntl.h>
#include <sys/ioctl.h>
#include <gtk/gtk.h>

#define BUF_SIZE 1024
#define MAX_CLIENTS 50
#define THREAD_COUNT 4

typedef struct
{
    char  id[128];
    char username[128];
    char password[128];
    char email[128];
}Infos;

int connect_to_server(const char *address, const int port);
void write_infos_to_server(int socketClient,const Infos* info);
int read_server_int(int socketClient);
void write_server_int(int socketClient,int mess);
int sign_in(int socketClient,const gchar* username,const gchar* password,const gchar* email);
int sign_up(int socketClient,const gchar* username,const gchar* password,const gchar* email);
#endif
