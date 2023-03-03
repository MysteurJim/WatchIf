#include "server.h"

int init_server()
{
    int socketServer = socket(AF_INET, SOCK_STREAM,0);
	struct sockaddr_in addrServer;
	addrServer.sin_addr.s_addr = inet_addr("127.0.0.1");
	addrServer.sin_family = AF_INET;
	addrServer.sin_port = htons(6969);

	if(bind(socketServer, (const struct sockaddr *)&addrServer, sizeof(addrServer)) < 0)
    {
        errx(1,"Bind Error");
    }

    if(listen(socketServer,MAX_CLIENTS) < 0)
    {
        errx(1,"Listen Error");
    }

    return socketServer;

}

int new_connextion(int socketServer)
{
    struct sockaddr_in addrClient;
	socklen_t csize = sizeof(addrClient);
	int socketClient = accept(socketServer, (struct sockaddr *)&addrClient, &csize);
    if(socketClient == -1)
    {
        perror("accept()");
    }else{
        puts("New Connexion !");
    }
    return socketClient;
}

int read_client(int sock,char *buffer)
{
    int n = 0;
    if((n = recv(sock,buffer,BUF_SIZE-1, 0)) < 0 )
    {  
        perror("read_client()");
        n = 0;
    }
    buffer[n] = 0;
    return n;
}


static void remove_client(Client *clients, int to_remove, int *actual)
{
    /* we remove the client in the array */
    memmove(clients + to_remove, clients + to_remove + 1, (*actual - to_remove - 1) * sizeof(Client));
    /* number client - 1 */
    (*actual)--;
}

int main()
{
    int socketServer = init_server();

    
    char buffer[BUF_SIZE];
    Client clients[MAX_CLIENTS];

    int actual = 0;
    int max = socketServer;

    fd_set rdfs;

    while(1)
    {
        FD_ZERO(&rdfs);

        FD_SET(STDIN_FILENO, &rdfs);
        FD_SET(socketServer, &rdfs);

        for (int i = 0; i < actual; i++) {
            FD_SET(clients[i].sock, &rdfs);
        }

        if(select(max + 1, &rdfs,NULL,NULL,NULL) == -1)
        {
            perror("select()");
            exit(errno);
        }

        //Input dans le server -> stop
        if(FD_ISSET(STDIN_FILENO,&rdfs))
        {
            break;
        }
        //Input dans le socket du serveur -> Nouvelle connection
        else if (FD_ISSET(socketServer,&rdfs))
        {
            //New client
            int socketClient = new_connextion(socketServer);
            if(socketClient == -1)
                continue;

            if(read_client(socketClient,buffer) < 0)
            {
                //disconnected
                continue;
            }
            max = socketClient > max ? socketClient : max;

            FD_SET(socketClient,&rdfs);
            Client c = {socketClient,""};
            strncpy(c.name,buffer,BUF_SIZE-1);
            clients[actual] = c;
            actual++;
            printf("%s\n",clients[actual-1].name);
        }
        else
        {
            for(int i = 0;i < actual;i++)
            {
                if(FD_ISSET(clients[i].sock, &rdfs))
                {
                    //Client is talking
                    Client client = clients[i];
                    int c = read_client(client.sock,buffer);
                    if(c == 0)
                    {
                        //Client disconnected
                        close(client.sock);
                        remove_client(clients,i,&actual);
                        printf("User : %s  -> Leave the server\n",client.name);
                    }
                    else
                    {
                        //Do something with client' mess
                    }
                    break;
                }
                
            }
        }

    }
	close(socketServer);
	return 0;

}
