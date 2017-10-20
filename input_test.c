/*********************************************************
 Examples: 5.3
 Hashing to Disk
 /*********************************************************/

#include <stdio.h>
#include <stdlib.h>  // exit
#include <string.h>

#define FILENAME "e_5_3.dat"
#define TABSIZE 100
#define OFLOWSIZE 100
#define BUCKETSIZE 4 // structs per bucket
#define INSERT 1
#define SEARCH 0

typedef struct record RECORD;
struct record
{
    char key[50]; // a very simple record
    int  num;     // a random number
};

long hash ( char *key, int size);

int main(void)
{
    FILE *create_hash_file(char *filename);
    void search_or_insert(FILE *fp, int insertflag);
    
    FILE *fp;
    
    fp = create_hash_file(FILENAME);
    search_or_insert(fp, INSERT);
    search_or_insert(fp, SEARCH);
    
    return 0;
}

/*******************************************************
 Hash formula is the same as used in Chapter 3
    PRE: key, size
    POST: address
 */
long hash ( char *key, int size)
{
    long address = 0;
    for (   ; *key != '\0'; key++)
    {
        address += *key * *key * *key;
    }
    return address % size;
}

/********************************************************
 Create an empty hash file
    PRE: filename
    POST: fp
 */
FILE *create_hash_file(char *filename)
{
    FILE *fp;
    //RECORD hashtable[TABSIZE][BUCKETSIZE] = {{"", "", "", ""}};
    RECORD hashtable[TABSIZE][BUCKETSIZE] = {""};
    RECORD overflow[OFLOWSIZE] = {""};
    
    if ((fp = fopen(filename, "w+b")) == NULL)
    {
        
        printf("Could not open %s.\n", filename);
        exit(1);
    }
    
    // check for write errors just to be safe.
    if (fwrite(&hashtable[0][0], sizeof (RECORD), TABSIZE * BUCKETSIZE, fp) < TABSIZE)
    {
        printf("Hash table could not be created. Abort!\n");
        exit(2);
    }
    
    if (fwrite(overflow, sizeof (RECORD), OFLOWSIZE, fp) < OFLOWSIZE)
    {
        printf("Could not create overflow area. Abort!\n");
        exit(3);
    }
    rewind(fp);
    return fp;
}

/*****************************************************************
 Hashing to disk: search or insert
    PRE: fp, insertflag
    POST: nothing
 */
void search_or_insert(FILE *fp, int insertflag)
{
    void insert_record(char *key, long address, FILE *fp);
    void search_record(char *key, long address, FILE *fp);
    
    char line[100], *token;
    long address;
    
    printf("%s\n", insertflag ? "INSERT" : "SEARCH");
    while(printf("Enter key: "), gets(line), strcmp(line, "quit") != 0)
    {
        token = strtok(line, "\040\t");
        if (!token) continue;
        address = hash(line, 100);
        if (insertflag)
        {
            insert_record(token, address, fp);
        }
        else
        {
            search_record(token, address, fp);
        }
    }
    return;
}

/******************************************************
 Hashing to Disk: Insert
    PRE: key, address, fp
    POST: new item inserted if possible, otherwise 
          error messages displayed
 */
void insert_record(char *key, long address, FILE *fp)
{
    RECORD detect, temp;
    int i;
    
    strcpy(temp.key, key);
    temp.num = rand()%99 + 5000;
    // go to beginning of hash bucket
    if (fseek(fp, address * BUCKETSIZE * sizeof (RECORD), SEEK_SET) != 0)
    {
        printf("Fatal seek error! Abort!\n");
        exit(4);
    }
    // find first available slot in the bucket.
    for (i = 0; i < BUCKETSIZE; i++)
    {
        fread(&detect, sizeof(RECORD), 1, fp);
        if (*detect.key == '\0') // available slot
        {
            fseek(fp, -1L * sizeof(RECORD), SEEK_CUR);
            fwrite(&temp, sizeof (RECORD), 1, fp);
            printf("Record: %s :added to bucket %ld.\n", temp.key, address);
            return; // nothing left to do
        }
    }
    
    // bucket full: insert into the overflow area
    fseek(fp, TABSIZE * BUCKETSIZE * sizeof(RECORD), SEEK_SET);
    for (i = 0; i < OFLOWSIZE; i++)
    {
        fread(&detect, sizeof(RECORD), 1, fp);
        if (*detect.key == '\0') // available slot
        {
            fseek(fp, -1L * sizeof (RECORD), SEEK_CUR);
            fwrite(&temp, sizeof (RECORD), 1, fp);
            printf("Record: %s : added to the overflow slot %d.\n", temp.key, i);
            return; // nothing left to do
        }
    }
    // item not inserted!
    printf("Hash table overflow! Abort!\n");
    exit(5);
}

/******************************************************
 Hashing to Disk: Search
    PRE: key, address, fp
    POST: search results displayed
 */
void search_record(char *key, long address, FILE *fp)
{
    RECORD detect, temp;
    int i;
    
    if (fseek(fp, address * BUCKETSIZE * sizeof (RECORD), SEEK_SET) != 0)
    {
        printf("Fatal seek error! Abort");
        exit(4);
    }
    // find first available slot in bucket
    for (i = 0; i < BUCKETSIZE; i++)
    {
        fread(&detect, sizeof (RECORD), 1, fp);
        if (strcmp(detect.key, key) == 0) // found it!
        {
            printf("\t%s found \n\tat hash bucket %ld.\n", key, address);
            printf("\tRandom number is %d\n", detect.num);
            return; // nothing left to do
        }
    }
    // check the overflow area
    fseek(fp, TABSIZE * BUCKETSIZE * sizeof (RECORD), SEEK_SET);
    for (i = 0; i < OFLOWSIZE; i++)
    {
        fread(&detect, sizeof (RECORD), 1, fp);
        if (strcmp(detect.key, key) == 0) // found it!
        {
            printf("\t%s found in overflow area.\n", key);
            printf("\tRandom number is %d\n", detect.num);
            return; // nothing left to do
        }
    }
    // not found
    printf("Records with key: %s : not found.\n", key);
    return;
}

/*
Cross reference listing made at: 11:39PM Monday; 07 August, 2017

BUCKETSIZE          13      64

FILE                28      29      31      60      62      95      97      98
127     176

FILENAME            10      33

INSERT              14      34

L                   146     160

OFLOWSIZE           12      65      81

RECORD              17      64      65      75      81      129     143     147
157     161     178     189     201

SEARCH              15      35

TABSIZE             11      64      75      154     198

address             47      50      52      97      98      101     108     111
115     127     135     176     181

char                20      24      28      45      60      97      98      100
127     176

continue            107

create_hash_file    28      33      60

define              10      11      12      13      14      15

detect              129     143     144     157     158     178     189     190
201     202

else                113

exit                71      78      84      138     168     184

filename            28      60      67

fopen               67

for                 48      141     155     187     199

fp                  29      31      33      34      35      62      67      81
86      87      95      97      98      111     115     127     135     143
146     147     154     157     160     161     176     181     189     198
201

fread               143     157     189     201

fseek               135     146     154     160     181     198

fwrite              75      81      147     161

hash                24      45      108

hashtable           64      75

i                   130     141     155     179     187     199

if                  67      75      81      107     109     135     144     158
181     190     202

include             6       7       8

insert_record       97      111     127

insertflag          29      95      109

int                 21      24      26      29      45      95      130     179


key                 20      24      45      48      50      97      98      127
132     144     158     176     190     202

line                100     106     108

long                24      45      47      97      98      101     127     176


main                26

num                 21      133

overflow            65      81

printf              70      77      83      103     104     137     148     162
167     183     192     193     204     205     210

rand                133

record              17      18

return              37      52      87      118     149     163     194     206
211

rewind              86

search_or_insert    29      34      35      95

search_record       98      115     176

size                24      45      52

sizeof              75      81      143     147     157     161     189     201


strcmp              190     202

strcpy              132

strtok              106

struct              17      18

temp                129     132     133     147     161     178

token               100     106     107     111     115

typedef             17

void                26      29      95      97      98      127     176

while               104

Memory Leak
Press any key to continue . . .
*/

