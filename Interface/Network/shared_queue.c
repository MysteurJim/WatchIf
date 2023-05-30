#include <err.h>
#include <stdlib.h>
#include "shared_queue.h"

shared_queue* shared_queue_new()
{
    shared_queue* sq = malloc(sizeof(struct shared_queue));
    if(sq == NULL)
    {
        err(EXIT_FAILURE,"Error on memory allocation");
    }
    sem_t lock;
    if(sem_init(&lock,0,1) == -1)
    {
        err(EXIT_FAILURE,"Error on sem_init()");
    }
    sq->lock = lock;

    sem_t size;
    if(sem_init(&size,0,0))
    {
        err(EXIT_FAILURE,"Error on sem_init()");
    }
    sq->size = size;

    sq->queue = NULL;
    return sq;
}

void shared_queue_push(shared_queue* sq, int val)
{
    sem_wait(&sq->lock);
    sq->queue = queue_push(sq->queue,val);
    sem_post(&sq->lock);
    sem_post(&sq->size);

}

int shared_queue_pop(shared_queue* sq)
{
    sem_wait(&sq->size);
    sem_wait(&sq->lock);
    int pval;
    sq->queue = queue_pop(sq->queue,&pval);
    sem_post(&sq->lock);
    return pval;
}

void shared_queue_destroy(shared_queue* sq)
{
    queue_empty(&(sq->queue));
    sem_destroy(&(sq->lock));
    sem_destroy(&(sq->size));
    free(sq);
}
