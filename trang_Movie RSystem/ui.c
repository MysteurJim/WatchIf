//Contains code for command line user interface
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<sqlite3.h>

#include "recommender.h"
#include "utility_matrix.h"

#define No_of_movies 9125

void ratemovie(int uid){
	int movieid;
	double rating;
	FILE *fstream = fopen("\\Dataset\\ratings_learn.csv","a");
	printf("Enter movieid: ");
	if(1 != scanf("%d",&movieid)){
        printf("\nCharacter inputs are not accepted.\n");
        return;
    }
    if(movieid < 1 || movieid > 9125){
		printf("Non existent movie id.");
		return;
	}
	printf("Enter rating: ");
	if(1 != scanf("%lf",&rating)){
        printf("\nCharacter inputs are not accepted.\n");
        return;
    }
    if(rating < 1.0 || rating > 5.0){
		printf("Invalid rating.\n");
		return;
	}
	fprintf(fstream,"\n%d,%d,%.1f",uid,movieid,rating);
	fclose(fstream);
}

int assignuid(){
    char *line, *record;
    char tmp[1024];
    FILE *fstream = fopen("\\Dataset\\ratings_learn.csv","r");
    int j=0;
    int max = 0;
    while((line=fgets(tmp,sizeof(tmp),fstream))!=NULL){
    record = strtok(line,",");
    while(record!=NULL){
        if(j==0){
            int t = atoi(record);
        if(t > max) max = t;
        }
        j++;
            record = strtok(NULL,","); //iterate
    }
    j=0;
    }
    fclose(fstream);
    free(line);
    free(record);
    return max+1;
}

int get_user_rates(void* NotUsed,int argc, char **argv,__attribute__((unused)) char **azColName)
{
    printf("%.1f - %s %s\n",atof(argv[1]),get_movie_names(atoi(argv[0])),get_movie_genres(atoi(argv[0])));
    return 0;
}


void getmovies(int uid){

    char tmp[1024];

	sqlite3 *db;
    char *err_msg = 0;

    int rc = sqlite3_open("Dataset/database.db", &db);
    
    if (rc != SQLITE_OK) {
        
        fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        
    }


	sprintf(tmp,"SELECT movies_id,Grade FROM Rates WHERE users_id = %d",uid);
	rc = sqlite3_exec(db, tmp, get_user_rates, NULL, &err_msg);
    
    if (rc != SQLITE_OK ) {
        
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);
        sqlite3_close(db);
        
    }

	//printf("%s",res);
	sqlite3_close(db);
    //return res;
}

void main(){
    int repeat = 1;
    int ans;
    int uid;
    int choice=0;
    int ans1;
    while(repeat==1){
        printf("\t\tMenu\nPlease enter one of the following choices:\n\t\t1 Help\n\n\t\t2 Login\n\n");
        if(1 != scanf("%d",&ans1)){
            printf("\nCharacter inputs are not accepted.\n");
            exit(12);
        }
        switch(ans1){
        case 1:
            printf("\nHello, following are the instructions to use this movie recommendation system!\n");
            printf("To rate a movie:\n1. Open movies.csv file\n2. Note the movie id of movie you want to rate\n3. Enter movie id and ratings\n");
            printf("Note: Movie ratings are out of 5.\n\n");
            break;
        case 2:
            repeat = 0;
            break;
        default:
            printf("Invalid choice\n");
        }
    }
    printf("Are you an existing user(Type '0') or a new user(Type '1'): ");
    if(1 != scanf("%d",&ans)){
        printf("\nCharacter inputs are not accepted.\n");
        exit(42);
    }
    int count = findusers();
    switch(ans){
        case 0:
            printf("Enter your user id: ");
            repeat = 1;
            while (repeat == 1){
                if(1 != scanf("%d",&uid)){
                    printf("\nCharacter inputs are not accepted. Please enter correct User ID:\n");
                    break;
                }
                else if(uid<1 || uid >= count){
                    printf("Invalid user id. Please enter correct id: \n");
                }
                else repeat = 0;
            }
            break;
        case 1:
            uid = findusers();
            printf("Your new user id is: %d\n",uid);
            break;
        default:
            printf("Enter valid input.\n");
            exit(84);
    }
    printf("Movies that you have rated:\n");
	getmovies(uid);
    repeat = 1;
    while (repeat == 1)
    {
        printf("\nDo you want to:\n0. Rate movies(Type '0')\n1. Show existing recommendations for your user id (Type '1')\n2. Movies that you have rated (Type '2')\n3. Exit(Type '3')\n");
        if (scanf("%d",&ans) == 1)
	{
        	if(isalpha(ans))
		{
           		printf("\nCharacter inputs are not accepted.\n");
            		break;
       		}
        	switch(ans)
		{
        		case 0:
            			while(choice == 0)
				{
                			ratemovie(uid);
                			printf("Rate more(Type '0') or recommend(Type '1'): ");
                			if (scanf("%d",&choice) != 1)
					{
						printf("Failed to read integer.\n");
					}
				}
            			recommender(uid);
            			break;
        		case 1:
            			recommender(uid);
            			break;
        		case 2:
            			getmovies(uid);
            			break;
        		case 3:
            			repeat = 0;
            			break;
        		default:
            			printf("Enter valid input.\n");
        	}
    	}
    }
}
