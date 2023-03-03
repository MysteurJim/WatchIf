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


int connect_to_server(const char *address, const int port)
{
	int socketClient= socket(AF_INET, SOCK_STREAM,0);
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

void write_to_server(int sock, const char* buffer)
{
	if(send(sock,buffer,strlen(buffer),0) < 0)
	{
		perror("write_to_server()");
		exit(errno);
	}
}

int main(void)
{
	int socketClient = connect_to_server("127.0.0.1",6969);
	char buffer[1024];
	printf("Entrez votre identifiant: ");
	
	for(int i = 0; i< 2 ; i++)
	{
		int n = scanf("%s",buffer);
		write_to_server(socketClient,buffer);
	}
	close(socketClient);

	return 0;
}
