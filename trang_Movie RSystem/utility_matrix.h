//header for utility_matrix.h
#ifndef UTILITY_MATRIX_H
#define UTILITY_MATRIX_H

char* get_movie_names(int index_movie);

char* get_movie_genres(int index_movie);

void get_utility_matrix();

void new_user_movies(double *newuser,int uid);
//defined as double *newuser = (double *)malloc(sizeof(double) * No_of_movies); where ratings of new user will be stored
#endif
