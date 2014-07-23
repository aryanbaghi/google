#ifndef _HASH_H
#define _HASH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "postingList.h"


typedef struct HashCell
{
    char *key;
    struct HashCell *next;
    PostingListStruct *postingList;
} HashCellStruct;

typedef struct HashTable 
{
    HashCellStruct *rows;
} HashTableStruct;

void insert(char **key, int keySize, PostingStruct *data, HashTableStruct *hashTable);

HashCellStruct* get_cell_ptr(unsigned int index, char **key, HashTableStruct *hashTable);
unsigned int hash(char **key, int keySize);

void free_hash_cell(HashCellStruct* hashCell);
void custom_free_hash_table(HashTableStruct* hashTable);

void hash_report(HashTableStruct* hashTable);

#endif

