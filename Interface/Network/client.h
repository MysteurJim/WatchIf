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
    char email[2048];
}Infos;

typedef struct
{
    int id;
    char Title[1024];
    int  Year;
    float Rate;
}Movie;

int connect_to_server(const char *address, const int port);
void write_infos_to_server(int socketClient,const Infos* info);
int read_server_int(int socketClient);
int read_server_info(int sockClient,Infos* info);
int read_server(int sock,char *buffer);
int read_server_movie(int sockClient,Movie* mov);
void write_server_int(int socketClient,int mess);
void write_server(int sock,const char *buffer);
int sign_in(int socketClient,const gchar* username,const gchar* password,const gchar* email);
int sign_up(int socketClient,const gchar* username,const gchar* password,const gchar* email);
#endif
