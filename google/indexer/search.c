#include "search.h"
#include "stem.h"
#include "hash.h"

#include <math.h>
#include <ctype.h>

#define BUFFER_SIZE (256 * 1024 * 1024)  
const unsigned int HASH_BYTE = 3;
const unsigned long long  HASH_SIZE = 16777216;
const int FAST_MODE = 0;

#define K 1.2
#define B 0.75
#define LDLAVG 1


typedef struct Score {
    unsigned int score;
    int docId;
    struct Score *next;
} ScoreStruct;
int len(unsigned int a)
{
    int result = 1;
    while(a>=10){
        a/=10;
        result++;
    }
    return result;
}

char * getContent(unsigned int position)
{
    FILE *invertedIndex;
    char *result;
    if(position > 100)
        position -= 100;
    result = (char *)calloc(200,1);
    invertedIndex = fopen("file.txt", "r");
    fseek(invertedIndex,position,SEEK_SET);
    fread(result, 1, 200, invertedIndex);
    fclose(invertedIndex);
    return result;
}

void load_from_file(char* filename, char **query, unsigned int size, HashTableStruct *hashTable, PostingStruct *docId)
{
    FILE *indexd;
    char len_char[4], *buffer, *token;
    unsigned int len_int, start=0, end=0,i=0,freq;
    PostingStruct data;
    PostingStruct *backDocId=docId;
    hashTable->rows = (HashCellStruct *) malloc(HASH_SIZE * sizeof(HashCellStruct));
    indexd = fopen(filename, "r");
    fread(len_char, 1, 4, indexd);
    memcpy(&len_int, len_char, 4);
    buffer = (char*) calloc(len_int + 1, sizeof(char));
    fread(buffer, 1, len_int, indexd);
    while(start<len_int){
        sscanf(buffer+start,"%u,",&docId->position);
        start+= len(docId->position)+1;
        // printf("%u\n",docId->position);
        docId->next = (PostingStruct *)calloc(1, sizeof(PostingStruct));
        backDocId = docId;
        docId = docId->next;
    }
    free(buffer);
    free(docId);
    backDocId->next=NULL;
    while(feof(indexd)==0){
        start = 0;
        end = 0;
        fread(len_char, 1, 4, indexd);
        memcpy(&len_int, len_char, 4);
        buffer = (char*) calloc(len_int + 1, sizeof(char));
        fread(buffer, 1, len_int, indexd);
        for(end=0;end<len_int;end++){
            if(buffer[end]=='#'){
                token = (char*) calloc(end-start+1,1);
                memcpy(token, buffer+start, end-start);
                end++;
                // printf("%s\n", token);
                for(i=0;i<size;i++){
                    if(strcmp(query[i],token)==0){
                        // printf("%s\n",buffer);
                        while(buffer[start]!='\n'){
                            // end++; //Skip first semicolon
                            start = end;
                            sscanf(buffer+start, "%u:", &data.docId);
                            start+=len(data.docId)+2;
                            end=start;
                            while(buffer[start]!='T'){
                                sscanf(buffer+start,"%u,", &data.position);
                                start+=len(data.position)+1;
                                end = start;
                                insert(&token, strlen(token), &data, hashTable);
                                // printf("DocID:%d \t Token:%s \tPos:%d\n",data.docId,token,data.position);
                            }
                            sscanf(buffer+start,"TF@%u", &freq);                            
                            start+= len(freq)+3;
                            end = start;
                        }
                        //scanf("%u",&data.position);
                    }
                    // printf("end %s\n",buffer+end);
                    
                    // if(buffer[end]!='\0')
                    //     end++;
                    // start=end;
                }
                while(buffer[end]!='\n')
                        end++;
                end++;
                start=end;
                free(token);
                
            }
        }
        free(buffer);
    }
    fclose(indexd);

}

unsigned int n(char* term, HashTableStruct* hashTable)
{
    unsigned int result = 0;
    HashCellStruct *cell = get_cell_ptr(hash(&term,strlen(term)), &term, hashTable);
    PostingStruct *cursor = cell->postingList->first;
    while(cursor!=NULL){
        result++;
        cursor = cursor->next;
    }
    return result;
}

double IDF(char* term, unsigned int N, HashTableStruct* hashTable)
{
    unsigned int n_of_term = n(term, hashTable);
    return log((N - n_of_term + 0.5) / (n_of_term + 0.5));
}
unsigned int f(char *term,unsigned int docId, HashTableStruct* hashTable)
{
    unsigned int result = 0;
    HashCellStruct *cell = get_cell_ptr(hash(&term,strlen(term)), &term, hashTable);
    PostingStruct *cursor = cell->postingList->first;
    while(cursor!=NULL){
        if(cursor->docId == docId)
            result++;
        cursor = cursor->next;
    }
    return result;
}

double bm25_score(unsigned int docId, char **query, unsigned int size,  HashTableStruct* hashTable)
{
    double score = 0;
    unsigned int i = 0;
    for(i=0;i<size;i++){
        score += IDF(query[i],2, hashTable)*f(query[i], docId, hashTable) / (f(query[i], docId, hashTable) + K * (1 - B + B * LDLAVG)); //beja e 2 bayad tedade kole sanada hesab beshe
    }
    return score;
}

void search(char* query)
{
    HashTableStruct hashTable;
    HashCellStruct *cell;
    PostingStruct *post;
    PostingStruct docId,*docIdCursor;
    char **parsed_query;
    char *doc;
    ScoreStruct score,*temp,*backtemp;
    score.docId = -1;
    int flag = 0;
    unsigned int num_of_term_in_query = 1, cursor = 0, first = 0,i = 0,k;
    while(query[cursor]!='\0'){
        if(query[cursor] == ' ' && isalnum(query[cursor+1]) > 0 ){
            num_of_term_in_query++;
        }
        cursor++;
    }
    parsed_query = (char **) malloc(sizeof(char*)*num_of_term_in_query);
    cursor = 0;
    i = 0;
    while(query[cursor]!='\0'){
        if(query[cursor]==' '){
            parsed_query[i++] = calloc(cursor-first+1,sizeof(char*));
            memcpy(parsed_query[i-1],&query[first],cursor-first);
            cursor++;
            first = cursor;
            continue;
        }
        cursor++;
    }
    parsed_query[i++] = calloc(cursor-first+1,sizeof(char*));
    memcpy(parsed_query[i-1],&query[first],cursor-first);
    cursor++;
    load_from_file("files/0",parsed_query, num_of_term_in_query, &hashTable, &docId);
    temp = &score;
    for(i=0;i<num_of_term_in_query;i++){
        cell = get_cell_ptr(hash(&parsed_query[i],strlen(parsed_query[i])), &parsed_query[i], &hashTable);
        if(cell!=NULL){
            post = cell->postingList->first;
            temp = &score;
            while(post!=NULL){
                flag = 0;
                temp = &score;
                backtemp = temp;
                // printf("HERE\n");
                while(temp!=NULL){
                    if( temp->docId==-1)
                        break;
                    if(temp->docId == post->docId){
                        flag = 1;
                        break;
                    }
                    backtemp = temp;
                    temp = temp->next;
                }
                if(flag==0){
                    // backtemp->score = bm25_score(post->docId, parsed_query, num_of_term_in_query, &hashTable);
                    backtemp->docId = post->docId;
                    docIdCursor = &docId;
                    for(k=1;k<=post->docId;k++)
                        docIdCursor = docIdCursor->next;
                    // printf("%u\n",docIdCursor->position);
                    doc = getContent(docIdCursor->position); // get from doc begining
                    // printf("%u\n", post->position);
                    doc = getContent(post->position);
                    // printf("%d\n", post->position);
                    printf("content@:@%s@@@##@@@score@:@%d@@@##@@@title@:@no@@##@@id:%u##$$##",doc,backtemp->score,post->docId);
                    free(doc);
                    backtemp->next = (ScoreStruct *) malloc(sizeof(ScoreStruct));
                    backtemp = backtemp->next;
                    backtemp->docId = -1;
                }
                post = post->next;
            }
        }
    }

}

int main(int argc, char **argv)
{
    if(argc!=2){
        printf("Invalid Query!\n");
        return -1;
    }
    search(argv[1]);
    return 0;
}
