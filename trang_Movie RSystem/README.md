# Movie-Recommendation-System
Implementation of movie recommendation system using Collaborative Filtering Technique using C language.

# Algorithm:

1. Creation of utility matrix of ratings between users and movies.
2. Finding similarity of new user (or concerned user) with other users using Centered Cosine Similarity (Pearson's Correlation).
3. Clustering of users based on similarity using k means clustering.
4. Prediction of movies using Collaborative Filtering technique (Low Rank Matrix Factorization) based on clusters obtained in step 3.
5. Top 10 movies printed on command line.

Instructions to run this system:
1. Open command line and locate the directory where source code is stored and type: `gcc ui.c recommender.c kmeans.c matrix_normalization.c pearsons.c predictions.c sorting.c utility_matrix.c -g -lm -lsqlite3`
2. Type `a` on command line to execute the program.

