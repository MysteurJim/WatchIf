#include <err.h>
#include <stdlib.h>
#include "queue.h"

queue* queue_push(queue* start, int val)
{
    queue* q = malloc(sizeof(struct queue));
    if(q == NULL)
    {
        err(EXIT_FAILURE,"Error on memory allocation");
    }
    q->val = val;

    if(start == NULL)
    {
        q->next = q;
    }else
    {
        q->next = start->next;
        start->next = q;
    }
    return q;
}

queue* queue_pop(queue* start, int *pval)
{
    if(start == NULL)
    {
        return NULL;
    }
    queue* q = start->next;
    *pval = q->val;
    if(start->next == start)
    {
        start =  NULL;
    }
    else
    {
        start->next = q->next;
    }
    free(q);
    return start;
}

void queue_empty(queue** pstart)
{
    while(*pstart != NULL)
    {
        int pval;
        *pstart = queue_pop(*pstart,&pval);
    }
}
