#include "hash.h"

#define DOCUMENT_SEPARATOR "<doc>"
extern const unsigned int HASH_BYTE;
extern const unsigned long long  HASH_SIZE;


unsigned int hash(char **key, int keySize)
{
    int i,j;
    unsigned int index=0;// = (unsigned int *) calloc (1, sizeof(unsigned int)); // HASH_BYTE should not be more than 4
    char *hashedKey = (char *) calloc (HASH_BYTE , sizeof(char)); 
    for(i=0;i<keySize;i+=HASH_BYTE)
        for(j=0;j<HASH_BYTE && (i+j)<keySize;j++)
            hashedKey[j] ^= (*key)[i+j] * (j+i);
    memcpy(&index,hashedKey, HASH_BYTE);
    free(hashedKey);
    return index;
}

HashCellStruct* get_cell_ptr(unsigned int index, char **key, HashTableStruct *hashTable)
{
    HashCellStruct *currentCell = &(hashTable->rows[index]);
    if(currentCell->key==NULL) {
        return NULL;
    }
    if(currentCell!=NULL)
        while(strcmp(currentCell->key, *key)!=0){
            currentCell = currentCell->next;
            if(currentCell==NULL)
                break;
        }
             
    return currentCell;
}

void insert(char **key, int keySize, PostingStruct *data, HashTableStruct *hashTable)
{
    unsigned int index = hash(key,keySize);
    HashCellStruct *cell = get_cell_ptr(index, key, hashTable);
    if(cell==NULL) {
        if(hashTable->rows[index].key==NULL) {
            hashTable->rows[index].key = (char *) calloc(keySize+1 , sizeof(char));
            strcpy(hashTable->rows[index].key, *key);
            hashTable->rows[index].next = NULL;
            hashTable->rows[index].postingList = (PostingListStruct *) malloc(sizeof(PostingListStruct));
            hashTable->rows[index].postingList->first = hashTable->rows[index].postingList->last = NULL;
            hashTable->rows[index].postingList->length = 0;
            append(hashTable->rows[index].postingList,data);
        }
        else { //conflict for first time /// optimize av
            cell = &hashTable->rows[index];
            while(cell->next!=NULL)
                cell = cell->next;
            cell->next = (HashCellStruct *) malloc(sizeof(HashCellStruct));
            cell = cell->next;
            cell->key = (char *) calloc(keySize+1 , sizeof(char));
            strcpy(cell->key, *key);
            cell->next = NULL;
            cell->postingList = (PostingListStruct *) malloc(sizeof(PostingListStruct));
            cell->postingList->first = cell->postingList->last = NULL;
            cell->postingList->length = 0;
            append(cell->postingList,data);
        } 
    } 
    else {
        append(cell->postingList,data);
    }
}

void hash_report(HashTableStruct* hashTable)
{
    unsigned long long int i = 0;
    unsigned long long int empty = 0;
    unsigned long long int conflict = 0;
    unsigned long long int total_conflict = 0;
    HashCellStruct *ptr;
    while(1){
        if(i==HASH_SIZE)
            break;
        if(hashTable->rows[i].key==NULL){
            empty++;
        }
        else{
            ptr = &hashTable->rows[i];
            while(ptr->next!=NULL)
            {
                ptr = ptr->next;
                conflict++;
            }
            if (conflict > 0)
                printf("conflict %llu: %llu\n",i,conflict);
            total_conflict += conflict;
            conflict = 0;
        }
        i++;
    }
    printf("empty: %llu\n conflict: %llu\n",empty,total_conflict);
}

void free_hash_cell(HashCellStruct* hashCell)
{
    HashCellStruct* next = NULL;
    if(hashCell!=NULL)
    {
        if(hashCell->next!=NULL)
            next = hashCell->next;
        if(hashCell->key!=NULL)
            free(hashCell->key);
        if(hashCell->postingList!=NULL)
            free_posting_list(hashCell->postingList);
        if(next!=NULL)
            return free_hash_cell(next);
    }
}
void custom_free_hash_table(HashTableStruct* hashTable)
{
    unsigned long int i=HASH_SIZE;
//    for(i=HASH_SIZE;i>0;i--){
//            if(hashTable->rows[i-1].key!=NULL){
//                free_hash_cell(&hashTable->rows[i-1]);}
//    }
}
