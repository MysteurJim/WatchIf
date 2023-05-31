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
    if(send(sock, buffer, 2048-1, 0) < 0)
    {
        perror("send()");
        exit(errno);
    }
}

void write_infos_to_client(int socketClient, const Infos* info)
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

void write_movie_to_client(int socketClient, Movie* mov)
{
	int n;
	do
	{
		if((n = send(socketClient,mov,sizeof(Movie),0)) < 0)
		{
			perror("write_to_server()");
			exit(errno);
		}
        printf("%i\n",n);
	}while(n != sizeof(Movie));
}

int read_client(int sock,char *buffer)
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

int get_movies_watch (void* NotUsed, int argc, char **argv,char **azColName)
{
    char* res = (char*)NotUsed;
    strcpy(res,argv[0]);
 
    return 0;
}
int litle_callback(void* NotUsed, int argc, char **argv,char **azColName)
{
    int* res = (int*)NotUsed;
    *res = atoi(argv[0]);
    return 0;
}
char* read_db_Movies_Watched(Infos* info)
{
    sqlite3 *db;
    char *err_msg = 0;
    char pass[BUF_SIZE];
    char idrequest[BUF_SIZE];
    char* res = malloc(2048);
    int rc = sqlite3_open("DataBase/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return;
    }
    int uid;
    sprintf(idrequest,"SELECT id FROM Users WHERE Name = '%s'",info->username);
    sqlite3_exec(db,idrequest , litle_callback, &uid, &err_msg);
    sprintf(pass,"SELECT GROUP_CONCAT(m.Title,'|') FROM Rates r JOIN Movies m ON r.movies_id = m.id WHERE r.users_id = %i",uid);
    
    rc = sqlite3_exec(db, pass, get_movies_watch, res, &err_msg);

    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return;
    } 
    sqlite3_close(db);
    return res;
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



void request_movies(int sock)
{
    Infos* info = malloc(sizeof(Infos));
    if(read_client_info(sock,info) < 0)
    {
        //disconnected
        close(sock);
        return;
    }
    
    char* res = read_db_Movies_Watched(info);
    write_client(sock,res);


}

int get_movies_search(void* NotUsed, int argc, char **argv,char **azColName)
{
    Movie* res = (Movie*)NotUsed;
    res->id = -1;
    if(argc > 0)
    {
        res->id = atoi(argv[0]);
        strcpy(res->Title,argv[1]);
        
        res->Year = atoi(argv[2]);
        res->Rate = atof(argv[3]);
    }
    return 0;
}

Movie* read_db_Movies_Searched(char* buf)
{
    sqlite3 *db;
    char *err_msg = 0;
    char pass[BUF_SIZE];
    Movie* mov = malloc(sizeof(Movie));
    int rc = sqlite3_open("DataBase/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
        return;
    }
    
    sprintf(pass,"SELECT * FROM Movies WHERE Title = '%s'",buf);
    
    rc = sqlite3_exec(db, pass, get_movies_search, mov, &err_msg);

    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
        return;
    } 
    sqlite3_close(db);
    return mov;
}

void search_movie(int sock)
{
    char* buf = malloc(2048);
    read_client(sock,buf);
    Movie* mov = read_db_Movies_Searched(buf);
    printf("%s\n",mov->Title);
    write_movie_to_client(sock,mov);
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
            int n = read_client_int(res);
            if(n == 0)
            {  
                sign_in(res);
            }else if(n == 1)
            {
                sign_up(res);
            }else if(n==2)
            {
                request_movies(res);
            }else
            {
                search_movie(res);
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
