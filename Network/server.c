#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>


typedef struct Data
{
	char id[50];
	char pass[50];
	int flag;
}Data;


Data parser(char buff[255])
{
	Data d;
	char f = 0;
	int j =0;
	for(int i = 0;i < 255;i++)
	{
		if(buff[i] == 0)
		{
			d.pass[j-1] = 0;
			break;
		}
		if(buff[i] == ',')
		{
			d.id[j] = 0;
			j = -1;
			f=1;
		}else{
			if(f)
			{
				d.pass[j] = buff[i];
			}else{
				d.id[j] = buff[i];
			}
		}
		j++;
	}
	return d;
}

void copy_array(char arr[], char arr2[])
{
	int i = 0;
	while(arr[i] !=0)
	{
		arr2[i] = arr[i];
		i++;
	}
	arr2[i] = 0;
}

int diff_array(char arr[],char arr2[])
{
	int i = 0;
	while(arr[i] != 0)
	{
		if(arr[i] != arr2[i])
			return 1;
		i++;
	}
	return 0;
}


void* identification(void* arg)
{
	FILE *fptr;
	char buff[255];
	char cop[255];
	int socket = *(int*)arg;
	int flag = 0;
	Data d = {
		.id = "Welcome to Pictu"
	};
	send(socket,&d,sizeof(Data),0); 
	printf("SEND : %s\n",d.id);

	recv(socket,&d,sizeof(Data),0);
	printf("Le client est : %s\n",d.id);



	fptr = fopen("temp/data","a+");

	if(fptr != NULL)
	{
		cop[0] = 1;
		while(fgets(buff, 255, (FILE*)fptr))
		{
			
			Data base = parser(buff);

			if(!diff_array(base.id,d.id))
			{
				if(!diff_array(base.pass,d.pass))
				{

					d.flag = 1;
					printf("L'utilisateur: %s est connecté !",d.id);

					send(socket,&d,sizeof(Data),0);
					break;
				}
				send(socket,&d,sizeof(Data),0);
			}
			copy_array(buff,cop);
		}
		
		//fprintf(fptr,"%s,%s\n",d.id,d.pass);
	}

	fclose(fptr);
	close(socket);//Pour plus tard a enlever
	free(arg);
	pthread_exit(NULL);
}

int main(void)
{

	pthread_t clientThread;
	int socketServer = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(30000);

	bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer));
	printf("bind : %d\n", socketServer);

	int socketClient;
	for(int i = 0;i<5;i++){
		listen(socketServer,5);
		printf("listen\n");

		struct sockaddr_in addrClient;
		socklen_t csize = sizeof(addrClient);
		socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
		printf("accept\n");

		printf("client: %d\n", socketClient);

		int *arg = malloc(sizeof(int));
		*arg = socketClient;
		pthread_create(&clientThread,NULL,identification,arg);
	}
	close(socketClient);
	close(socketServer);
	printf("close\n");
	return 0;

}
