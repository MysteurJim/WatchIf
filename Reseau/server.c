#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>


typedef struct Data
{
	char message[50];
}Data;



int main(void)
{
	int socketServer = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(30000);

	bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
	printf("bind : %d\n", socketServer);
	
	int socketClient;

	for(int i = 0;i<4;i++){
	listen(socketServer,5);
	printf("listen\n");

	struct sockaddr_in addrClient;
	socklen_t csize = sizeof(addrClient);
	socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
	printf("accept\n");

	printf("client: %d\n", socketClient);

	Data d = {
		.message = "Welcome to Pictu"
	};
	send(socketClient,&d,sizeof(Data),0); 
	printf("SEND : %s\n",d.message);

	recv(socketClient,&d,sizeof(Data),0);
	printf("Le client est : %s\n",d.message);

	}
	close(socketClient);
	close(socketServer);
	printf("close\n");
	return 0;

}
