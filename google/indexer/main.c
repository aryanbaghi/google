#include "hash.h"
#include "hashWriter.h"
#include "stem.h"
#include "merge.h"
#include "search.h"

#include <stdlib.h>

#define BUFFER_SIZE (256 * 1024 * 1024)  
const unsigned int HASH_BYTE = 3;
const unsigned long long  HASH_SIZE = 16777216;
const int FAST_MODE = 0;

void itoa(int num, char* buffer)
{
    char b[100];
    int j=0,i=0;
    for(;j<100;j++)
        buffer[j]='\0';
    do{
        buffer[i] = num % 10 + 48;
        i++;
        num /= 10;
    } while(num!=0);
    for(i=0;i<strlen(buffer);i++)
        b[i] = buffer[strlen(buffer)-i -1];
    for(i=0;i<strlen(buffer);i++)
        buffer[i] = b[i];
}

int main()
{
    FILE *corpus;
    FILE **allOutput;
    char a[100];
    char *token;
    char filename[100],addr[100];
    unsigned int i = 0,start=0;
    int need_to_cache = 0, counter = 0,template;
    HashTableStruct hashTable;
    PostingStruct data;
    PostingStruct docId;
    PostingStruct *docIdCursor = &docId;
    PostingStruct *docIdCursorTemp;
    unsigned int size;
    char *buffer, *lastWord;
    buffer = (char*) calloc(BUFFER_SIZE, sizeof(char));
    hashTable.rows = (HashCellStruct *) malloc(HASH_SIZE * sizeof(HashCellStruct));
    corpus = fopen("file.txt", "r");
    size = fread(buffer, 1, BUFFER_SIZE, corpus);
    printf("Start indexing ...\n");
    token = (char*) malloc(1);
    do{
        for(start=0,i=0;i<size;i++) {
            if(buffer[i]==' ' || buffer[i]=='\''|| buffer[i]==',' || buffer[i]=='\"' || buffer[i]=='\0' || buffer[i]=='\t' || buffer[i]=='\n' || i==size-1){
                if(i-start==0){
                    start++;
                    continue;
                }
                free(token);
                if(need_to_cache){
                    need_to_cache = 0;
                    token = (char *) calloc (strlen(lastWord)+i-start+1, sizeof(char)); 
                    memcpy(token,lastWord, strlen(lastWord));
                    data.position = start - strlen(lastWord);  
                    free(lastWord); 
                }
                else{
                    token = (char *) malloc (i-start+1);
                    token[i-start] = '\0';
                    memcpy(token,buffer+start,i-start);
                    data.position = start;
                }
                data.next = NULL;
                // printf("%d: %s\n",start,token);
                if(strcmp(token,"</XML>")==0){
//                    docIdCursor->position = i;
//                    docIdCursor->next = (PostingStruct *) malloc(sizeof(PostingStruct));
//                    docIdCursorTemp = docIdCursor;
//                    docIdCursor = docIdCursor->next;
                    start = i+1;
                    continue;
                }
                if(strcmp(token,"<XML>")==0){
                    docIdCursor->position = i;
                    docIdCursor->next = (PostingStruct *) malloc(sizeof(PostingStruct));
                    docIdCursorTemp = docIdCursor;
                    docIdCursor = docIdCursor->next;
                    start = i+1;
                    continue;
                }
                stem(token);
                if(strlen(token)>0){
                    insert(&token, i-start, &data, &hashTable);
                }
                start = i+1;
            }
            else if(i==size-1)
            {
                lastWord = (char*) malloc(size - start + 1);
                lastWord[size-start] = '\0';
                memcpy(lastWord,buffer+start,size-start);
                need_to_cache = 1;
            }
        }
        free(docIdCursor);
        docIdCursorTemp->next = NULL;
        // custom_free_hash_table(hashTable);
        itoa(counter,filename);
        strcpy(addr,"files/");
        strcat(addr,filename);
        write_hash(&hashTable, addr,"w", &docId);
        hash_report(&hashTable);
        custom_free_hash_table(&hashTable);
        free(hashTable.rows);
        hashTable.rows = (HashCellStruct *) malloc(HASH_SIZE * sizeof(HashCellStruct));
        size = fread(buffer, 1, BUFFER_SIZE, corpus);
        counter++;
    } while( size==BUFFER_SIZE); 
    fclose(corpus);
    printf("Indexing done!\n");
    free(buffer);
    custom_free_hash_table(&hashTable);
    allOutput = (FILE**) malloc (counter*sizeof(FILE*));
    docIdCursor = docId.next;
    if(docId.next!=NULL)
        while(docIdCursor->next!=NULL)
        {
            docIdCursorTemp = docIdCursor->next;
            free(docIdCursor); 
            docIdCursor = docIdCursorTemp;
        }
    printf("Start merging ...\n");
    if(counter>1){
        for(template=0;template<counter;template++){
            itoa(template,filename);
            strcpy(addr,"files/");
            strcat(addr,filename);
            allOutput[template] = fopen(addr,"r");
        }
        merge(allOutput, counter, "result.txt");
    }
    else
        printf("Output is files/0 \n");
    printf("Merging done!\n");
    // printf("Enter query:\n");
    // scanf("%[^\n]",a);
    // printf("query is: %s\n", a);
    // search(a);
    custom_free_hash_table(&hashTable);
    free(hashTable.rows);
    return 0;
}