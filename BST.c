#include "BST.h"

#define MAX 50

/****************************************************************
INORDER
Print a BST in Left-Root-Right sequence.
*/
void printTreeInorder(NODE *root)
{
	int num = 0;
	QUEUE *temp;
	if (root){
		printTreeInorder(root->left);

		//printf("%s:\t%d\n", root->wrd, root->count);
		printf("%-20s", root->wrd);
		while (root->firstLine)
		{
			temp = dequeue(&root->firstLine, &root->lastLine);
			printf("%-8d", temp->lineNum);
			free(temp);
			num++;

			if (num == 5)
			{
				num = 0;
				printf("\n\t\t    ");
			}
		}
		printf("\n\n");

		printTreeInorder(root->right);
	}
	return;
}

void destroyBST(NODE *root)
{
	QUEUE *temp;
	if (root) {
		destroyBST(root->left);
		destroyBST(root->right);
		while (temp = dequeue(&root->firstLine, &root->lastLine))
		{
			free(temp);
		}
		free(root);
	}
}

/****************************************************************
RECURSIVE Insert
ADD update count
****************************************************************/
int insert( NODE **root, char *wrd, int lineNum)
{
	//needs to enqueue somewhere
	if(!(*root)){ // parent found: insert data
		// allocate the new node
		if(!(*root = (NODE *) malloc (sizeof(NODE))))
			printf( "Fatal malloc error!\n" ), exit(1);

		(*root)->count = 1;
		strcpy((*root)->wrd, wrd);											//strcpy
		(*root)->firstLine = (*root)->lastLine = NULL;
		enqueue(&(*root)->firstLine, &(*root)->lastLine, lineNum);
		(*root)->left  = (*root)->right = NULL;
		return 1; // data inserted
	}

	if (strcmp(wrd,(*root)->wrd) > 0)										//strcmp
		return insert(&(*root)->right, wrd, lineNum);
	else if (strcmp(wrd, (*root)->wrd) < 0)									//strcmp
		return insert(&(*root)->left, wrd, lineNum);
	else if (strcmp(wrd, (*root)->wrd) == 0)
	{
		(*root)->count += 1;
		//if lineNum exists in the queue already, dont enqueue it
		if(lineNum != (*root)->lastLine->lineNum)
			enqueue(&(*root)->firstLine, &(*root)->lastLine, lineNum);
		//printf("Node already in the tree!\n");
		return 0; // duplicate
	}
}


/****************************************************************
RECURSIVE Search
****************************************************************/
NODE *find(NODE *root, char *target)
{
	if (root)
	{
		if (strcmp(target, root->wrd) > 0)								//strcmp
			return find(root->left, target);
		else if (strcmp(target, root->wrd) < 0)							//strcmp
			return find(root->right, target);
		else
			return root;
	}
	return NULL;
}