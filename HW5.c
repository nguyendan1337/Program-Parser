#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "BST.h"
#include "Queue.h"

#ifdef _MSC_VER
#include <crtdbg.h>  // needed to check for memory leaks (Windows only!)
#endif

#define DELIMITERS "~!#%&()-+=[]{}|:;,.? \040\t\n\\0123456789"
#define MEM_ERROR printf("Not enough memory!\n")

void parseFile(char *filename, NODE **root);
void printTime();

int main(int argc, char *argv[])
{
	NODE *root = NULL;
	
	printTime();
	parseFile(argv[1], &root);
	printTreeInorder(root);
	destroyBST(root);

	//check for memory leaks
	#ifdef _MSC_VER
	printf(_CrtDumpMemoryLeaks() ? "Memory Leak\n" : "No Memory Leak\n");
	#endif

	system("PAUSE");
	return 0;
}

void parseFile(char *filename, NODE **root)
{
	char line[200];
	char *token;
	int lineNum = 0;

	FILE *fin;
	fin = fopen(filename, "r");
	if (fin == NULL)
	{
		printf("Error opening input file\n");
		system("PAUSE");
		exit(200);
	}

	while (fgets(line, 200, fin))
	{
		lineNum++;

		token = strtok(line, DELIMITERS);
		while (token != NULL)
		{
			//if the token is inside quotes, ignore it
			if (token[0] == '\"' || token[0] == '\'')
			{
				token = NULL;
			}
			//if there is *
			else if (token[0] == '*')
			{
				//if its a single *, grab next token
				if (strlen(token) == 1)
					token = strtok(NULL, "~!#%&()-+=[]{}|:;,.? \040\t\n\\0123456789*");
				//else remove extra *s
				else token = strtok(token, "~!#%&()-+=[]{}|:;,.? \040\t\n\\0123456789*");
			}
			//if token is in brackets
			else if (token[0] == '<' || token[0] == '>')
			{
				token = NULL;
			}

			//if token is not NULL
			if (token != NULL)
			{
				//insert it
				if (token[0] != '/')
				{
					insert(root, token, lineNum);
					token = strtok(NULL, DELIMITERS);
				}
				//if the beginning of the token is //, go to next line
				else if (token[1] == '/')
				{
					token = NULL;
				}
				//if the beginning of the token is /*
				else if (token[1] == '*')
				{
					//search for */
					do
					{
						if (token[strlen(token) - 1] == '/' && token[strlen(token) - 2] == '*')
							token = NULL;
						else
						{
							while (token != NULL && !(token[strlen(token) - 1] == '/' && token[strlen(token) - 2] == '*'))
							{
								token = strtok(NULL, DELIMITERS);
							}

							if (token == NULL)
							{
								fgets(line, 200, fin);
								lineNum++;
								token = strtok(line, DELIMITERS);
							}
						}
					} while (token != NULL);
				}
			}
		}
	}

	fclose(fin);
}

void printTime()
{
	struct tm *timeptr;
	time_t timeval;
	char buffer[80];
	time(&timeval);
	timeptr = localtime(&timeval);

	printf("Cross reference listing made at: ");
	strftime(buffer, 80, "%I:%M%p %A; %d %B, %Y\n", timeptr);
	puts(buffer);
}

//OUTPUT
//input.c
/*
Cross reference listing made at: 04:45PM Tuesday; 08 August, 2017

DUMMY_TRAILER       11

NULL                140     160

REQUEST             16      130     160     192

SERVICE             25      36      38      39      41
61      63      66      125     127
128     129     140     190

char                19      28      37      42      106
108

curr                127     133     135     136     138
157     180     181

define              11

else                115     155     178

exit                70      76      85      91      143
148     163     168

for                 199

front               29

getinfo             37      46      106

gets                111

if                  67      73      82      88      113
138     140     145     160     165
174

include             7       8       9

init_list           36      45      61

insert              38      48      125

int                 34

job_id              20      43      46      48

line                108     111     113     116

long                20      43

main                34

malloc              66      140     160

newreq              130     160     165     170     171
172

newserv             129     140     145     150     151
153     157     174     176

next                21      31

prev                128     135     152

printf              69      75      84      90      110
142     147     162     167     197
198     201

putchar             203

qname               28      37      42      46      48
106     114     133     138

rear                30

reqptr              192     199

request             16      17      21      29      30


return              52      97      117     183

serv_ptr            38      39      41      45      48
50      63      66      67      72
73      78      81      82      87
88      93      94      95      97
125     190     194     195     199
204

service             25      26      31

sizeof              66      140     160

sscanf              116

strcmp              46      113     133     138

strcpy              114     150     170

struct              16      17      21      25      26
29      30      31

traverse            39      50      190

typedef             16      25

username            19      42      46      48

void                34      37      38      39      106
125     190

while               46      133     195

No Memory Leak
Press any key to continue . . .
*/