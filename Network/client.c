#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <err.h>


typedef struct Data
{
	char message[50];
}Data;

int main(void)
{
	int socketClient= socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in addrClient;
	addrClient.sin_addr.s_addr =  inet_addr("127.0.0.1");
	addrClient.sin_family = AF_INET;
	addrClient.sin_port = htons(30000);
	if(connect(socketClient,(const struct sockaddr *)&addrClient, sizeof(addrClient)))
	{
		err(1,"");
	}
		printf("Connect\n");

	Data d;
	recv(socketClient,&d,sizeof(Data),0);
	printf("%s \n",d.message);
	printf("Entrez votre identifiant: ");
	
	scanf("%s",&d.message);

	send(socketClient,&d,sizeof(Data),0);
	
	
	close(socketClient);

	return 0;
}
