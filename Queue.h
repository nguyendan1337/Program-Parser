#ifndef QUEUE_H
#define QUEUE_H

#include <stdio.h>
#include <stdlib.h>


typedef struct queue
{
	int          lineNum;
	struct queue *next;
}QUEUE;

QUEUE *dequeue(QUEUE **queue, QUEUE **rear);
void  enqueue(QUEUE **queue, QUEUE **rear, int lineNum);
void destroyQueue();

#endif