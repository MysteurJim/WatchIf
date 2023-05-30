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

void on_button1_clicked();
void on_button2_clicked();
int connect_to_server(const char *address, const int port);
void write_infos_to_server(int socketClient,const Infos* info);
int read_server_int();
void write_server_int(int mess);
#endif
