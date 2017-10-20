#include "Queue.h"

/***************************************************
Queue Insert: insert at the end
*/
void enqueue(QUEUE **queue, QUEUE **rear, int lineNum)
{
	QUEUE *pnew;

	pnew = (QUEUE *)malloc(sizeof (QUEUE));
	if (!pnew)
	{
		printf("... error in enqueue!\n");
		exit(1);
	}
	pnew->lineNum = lineNum;
	pnew->next = NULL;
	if (*queue == NULL) *queue = pnew;
	else (*rear)->next = pnew;
	*rear = pnew;
	return;
}

/***************************************************
Queue Delete: remove the first node
*/
QUEUE *dequeue(QUEUE **queue, QUEUE **rear)
{
	QUEUE *first;

	if (*queue == NULL) return NULL;
	first = *queue;
	*queue = (*queue)->next;
	if (*queue == NULL) *rear = NULL;
	first->next = NULL;

	return first;
}

void destroyQueue()
{

}