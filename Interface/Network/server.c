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


int read_client_int(int sock)
{
    int n = 0;
	int msg = 0;

	if((n = recv(sock,&msg,sizeof(int),0)) < 0 )
	{  
		perror("read_client()");
		n = 0;
	}
    return msg;
}

int read_client_info(int sock,Infos* info)
{
    int n = 0;
    if((n = recv(sock,info,sizeof(Infos),0)) < 0 )
	{
        perror("read_client()");
		n = 0;
    }
    return n;
}

static void write_client(int sock,const char *buffer)
{
    if(send(sock, buffer, strlen(buffer), 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
}

static void write_client_int(int sock,int mess)
{
    int n;
    if((n = send(sock, &mess, sizeof(int), 0)) < 0)
    {
        perror("send()");
        exit(errno);
    }
}


int callback (void* NotUsed, int argc, char **argv,char **azColName)
{
    Infos* c = (Infos*)NotUsed;
    
    strcpy(c->id,argv[0]);
    strcpy(c->username,argv[1]);
    strcpy(c->password,argv[2]);
    strcpy(c->email,argv[3]);
    
    //printf("Secret password: %s\n",argv[0]);
    return 0;
}

int read_db_Users(const char *buffer,Infos* c)
{
    sqlite3 *db;
    char *err_msg = 0;
    char pass[BUF_SIZE];
    int rc = sqlite3_open("DataBase/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    //sprintf(pass,"SELECT Password FROM Users WHERE Name = '%s'",buffer);
    sprintf(pass,"SELECT * FROM Users WHERE Name = '%s'",buffer);
    rc = sqlite3_exec(db, pass, callback, c, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return 1;
    } 
    //printf("User: %s Password: %s\n",buffer,res);
    sqlite3_close(db);
    return 0;
}

int write_db_Users(Infos* c)
{
    //FAIRE UN CHANGEMENT EN FONCTION DE LA DB QUON VEUT LIRE ?
    sqlite3 *db;
    char *err_msg = 0;
    char pass[BUF_SIZE];
    int rc = sqlite3_open("DataBase/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return 1;
    }
    
    //sprintf(pass,"SELECT Password FROM Users WHERE Name = '%s'",buffer);
    sprintf(pass,"INSERT INTO Users (Name,Password,Email) VALUES ('%s','%s','')",c->username,c->password);
    rc = sqlite3_exec(db, pass, NULL, NULL, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return 1;
    } 
    //printf("User: %s Password: %s\n",buffer,res);
    sqlite3_close(db);
    return 0;
}


void sign_up(int sock)
{
    Infos* info = malloc(sizeof(Infos));
    if(read_client_info(sock,info) < 0)
    {
        //disconnected
        close(sock);
        return;
    }
    write_db_Users(info);
    write_client_int(sock,0);
}

void sign_in(int sock)
{
    //char buffer[BUF_SIZE];
    Infos* info = malloc(sizeof(Infos));
    if(read_client_info(sock,info) < 0)
    {
        //disconnected
        close(sock);
        return;
    }
    
    char* username = info->username;
    char* mdp      = info->password;
    char* email    = info->email;
    
    //printf("User : %s Pass: %s\n Email: %s\n",username,mdp,email);

    //Test sur DB si User and Pass are good
    //char* pass = malloc(BUF_SIZE);
    Infos* client = malloc(sizeof(Infos));
    /*
    char* request = malloc(BUF_SIZE);
    sprintf(request,"SELECT Password FROM Users WHERE Name = '%s'",username);
    */
    if(read_db_Users(username,client) > 0)
    {
        write_client_int(sock,2);
    }
    else
    {
        if(strcmp(client->password,mdp) == 0)
        {
            //Si oui
            //-> Receive Data on Users and send on client.
            
            write_client_int(sock,0);
        }else
        {
            //Si non
            //-> Send to client Error  Only int ?
            write_client_int(sock,1);
        }
    }

    printf("ID: %s \n Name: %s \n Pass: %s \n Email: %s \n",client->id,client->username,client->password,client->email);
    
    
    close(sock);
    free(info);

}
// Function executed by the threads.
void* worker(void* arg)
{
	// Gets the shared queue.
	shared_queue* queue = arg;
	int res;
	while(1)
	{
		res = shared_queue_pop(queue);
        if(res != NULL)
        {
            if(read_client_int(res) == 0)
            {  
                printf("JE VAIS SIGN_IN\n");
                sign_in(res);
            }else
            {
                printf("JE VAIS SIGN_UP\n");
                sign_up(res);
            }
            
        }
	}
	
}

int main()
{
    int socketServer = init_server();
    
    

    shared_queue* queue = shared_queue_new();

    pthread_t thr1;
    for(size_t i = 0;i < THREAD_COUNT;i++)
	{
		if( pthread_create(&thr1,NULL,worker,(void*)queue) != 0)
		{
			err(EXIT_FAILURE,"pthread create()");
		}	
	}

    while(1)
    {
        int socketClient = new_connextion(socketServer);
        if(socketClient == -1)
            continue;

        shared_queue_push(queue,socketClient);

            
    }
	close(socketServer);
	return 0;

}
