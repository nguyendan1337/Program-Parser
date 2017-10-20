#ifndef BST_H
#define BST_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "Queue.h"



typedef struct nodeTag{
	char wrd[100];
	int count;
	struct nodeTag *left;
	struct nodeTag *right;
	QUEUE *firstLine;
	QUEUE *lastLine;
} NODE;

void printTreeInorder(NODE *root);
int insert(NODE **root, char *wrd, int lineNum);
NODE *find(NODE *root, char *target);
void destroyBST(NODE *root);

#endif