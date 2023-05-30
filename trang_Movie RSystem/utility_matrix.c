//creates utility matrix
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sqlite3.h>

#define No_of_movies 9123

int callback (void* NotUsed,__attribute__((unused)) int argc, char **argv,__attribute__((unused)) char **azColName)
{
	char* res = (char*) NotUsed;
	sprintf(res,"%s",argv[0]);
	
	return 0;
}

char* get_movie_names(int index_movie){

	char tmp[1024];
    
	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("Dataset/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
    }

	char* res = malloc(1024);
	sprintf(tmp,"SELECT Title FROM Movies WHERE id = %i",index_movie);
	rc = sqlite3_exec(db, tmp, callback, res, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
    } 
	sqlite3_close(db);
    return res;

}

int get_genres(void* NotUsed,int argc, char **argv,__attribute__((unused)) char **azColName)
{

	char* res = (char*) NotUsed;
	*res = 0;
	for(int i = 0;i<argc;i++)
	{
		sprintf(res,"%s",argv[i]);
	}
	return 0;
}

char* get_movie_genres(int index_movie){
	//Juste renvoyer une string avec le Genre des films cest pour l4output
	char tmp[1024];

	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("Dataset/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
    }

	char* res = malloc(1024);
	sprintf(tmp,"SELECT GROUP_CONCAT(g.Genre,'|')	FROM Movies m JOIN Genres g ON m.id = g.id_movies WHERE m.id = %d;",index_movie);
	rc = sqlite3_exec(db, tmp, get_genres, res, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
    }

	//printf("%s",res);
	sqlite3_close(db);
    return res;
}

int get_rates(void* NotUsed,int argc, char **argv,__attribute__((unused)) char **azColName)
{
	double* res = (double*) NotUsed;

	//printf("B %f\n",*(res + (atoi(argv[0])*No_of_movies + atoi(argv[1]))));
	//printf("%d\n",atoi(argv[0])*No_of_movies + atoi(argv[1]));
	//printf("A:%d B:%s\n",atoi(argv[1]),argv[1]);
	*(res + ((atoi(argv[0])-1)*No_of_movies + (atoi(argv[1]))-1)) = atof(argv[2]);
	//printf("A %f\n",*(res + (atoi(argv[0])*No_of_movies + atoi(argv[1]))));
	return 0;
}

void get_utility_matrix(double **utility_matrixe){
	char tmp[1024];
    double* utility_matrix = *utility_matrixe;
	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("Dataset/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

	
	sprintf(tmp,"SELECT * FROM Rates;");
	rc = sqlite3_exec(db, tmp, get_rates, utility_matrix, &err_msg);
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        return;
    } 
	sqlite3_close(db);
    
}

int get_user_rate(void* NotUsed,int argc, char **argv,__attribute__((unused)) char **azColName)
{
	double* res = (double*)NotUsed;
	*(res+atoi(argv[0])) = atof(argv[1]);
}

void new_user_movies(double *newuser, int uid){
	char tmp[1024];

	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("Dataset/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
    }


	sprintf(tmp,"SELECT movies_id,Grade FROM Rates WHERE users_id = %d",uid);
	rc = sqlite3_exec(db, tmp, get_user_rate, newuser, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
    }

	//printf("%s",res);
	sqlite3_close(db);
    //return res;
}

