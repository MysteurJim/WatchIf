#include "client.h"


int set_socket_timeout(int sockfd, int timeout_sec)
{
    struct timeval timeout;
    timeout.tv_sec = timeout_sec;
    timeout.tv_usec = 0;

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char *)&timeout, sizeof(timeout)) < 0)
    {
        perror("set_socket_timeout()");
        return -1;
    }

    return 0;
}


int connect_to_server(const char *address, const int port)
{
	int socketClient = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in addrClient;
	addrClient.sin_addr.s_addr =  inet_addr(address);
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(port);
	if(connect(socketClient,(const struct sockaddr *)&addrClient, sizeof(addrClient)))
	{
		perror("connect()");
	}
	printf("Connect\n");
	return socketClient;
}



void write_server_int(int socketClient,int mess)
{
    int n;
    if((n = send(socketClient, &mess, sizeof(int), 0)) < 0)
    {
        perror("send()");
        exit(errno);
    }
}

int read_server_int(int socketClient)
{
    int n = 0;
    int msg = 1;

    if (set_socket_timeout(socketClient, 5) < 0)
    {
        exit(EXIT_FAILURE); // Gestion de l'erreur de configuration du timeout
    }

    if ((n = recv(socketClient, &msg, sizeof(int), 0)) < 0)
    {
        if (errno == EAGAIN || errno == EWOULDBLOCK)
        {
            printf("Aucune donnée disponible dans le délai imparti.\n");
        }
        else
        {
            perror("read_server_int()");
            exit(errno);
        }
    }

    return msg;
}

int read_server_info(int sockClient,Infos* info)
{
    int n = 0;
    if((n = recv(sockClient,info,sizeof(Infos),0)) < 0 )
	{
        perror("read_client()");
		n = 0;
    }
    return n;
}

void write_infos_to_server(int socketClient, const Infos* info)
{
	int n;
	do
	{
		if((n = send(socketClient,info,sizeof(Infos),0)) < 0)
		{
			perror("write_to_server()");
			exit(errno);
		}
	}while(n != sizeof(Infos));
}

void write_server(int sock,const char *buffer)
{
    if(send(sock, buffer, 2048-1, 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
}

int read_server_movie(int sockClient,Movie* mov)
{
    int n = 0;
    do{
        if((n += recv(sockClient,mov,sizeof(mov),0)) < 0 )
        {
            perror("read_client()");
            n = 0;
            
        }
        printf("Receve :%d\n",n);
    }while(n < sizeof(Movie));
    return n;
}

int read_server(int sock,char *buffer)
{
    int n = 0;
    if((n = recv(sock,buffer,2048-1, 0)) < 0 )
    {  
        perror("read_client()");
        exit(errno);
    }
    buffer[n] = 0;
    return n;
}

int sign_in(int socketClient,const gchar* username,const gchar* password,const gchar* email)
{

	Infos* info = malloc(sizeof(Infos));
	strcpy(info->username,username);
	strcpy(info->password,password);
	strcpy(info->email,email);
	write_infos_to_server(socketClient,info);

	//CODE DE CONFIRMATION
	//0 -> Nice
	//1 ->  Incorrect username or password.
	int msg = read_server_int(socketClient);
	free(info);
	return msg;
}

int sign_up(int socketClient,const gchar* username,const gchar* password,const gchar* email)
{

	Infos* info = malloc(sizeof(Infos));
	strcpy(info->username,username);
	strcpy(info->password,password);
	strcpy(info->email,email);
	write_infos_to_server(socketClient,info);

	//CODE DE CONFIRMATION
	//0 -> Nice
	//1 ->  Incorrect username or password.
	int msg = read_server_int(socketClient);
	free(info);
	return msg;
}
