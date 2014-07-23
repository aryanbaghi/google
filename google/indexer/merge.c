#include "merge.h"

#define LINE_NUM 100000  
extern const unsigned long long  HASH_SIZE;
#define TERM_SEPARATOR '#'

int is_there_any_file_to_read(FILE **file, int size)
{
    int i=0;
    for(i=0;i<size;i++)
        if(!feof(file[i]))
            return 1;
    return 0;
}

int get_postion(char *buffer, int *cursor)
{
    /*char INT[256];
    int templateCursor = *cursor;
    while(buffer[templateCursor]!=',')
        templateCursor++;
    memcpy(INT, buffer+(*cursor), templateCursor - (*cursor));
    *cursor = templateCursor + 1; 
    return atoi(INT);*/
    int result=0;
    memcpy(&result, buffer+(*cursor),4);
    *cursor += 5;
    return result;
}
char* get_term(char *buffer, int *cursor)
{
    char *result;
    *cursor = 0;
    while(buffer[*cursor]!=TERM_SEPARATOR)
        (*cursor)++;
    result = (char*) malloc(((*cursor)+1) * sizeof(char) );
    result[(*cursor)] = '\0';
    memcpy(result,buffer, *cursor);
    (*cursor)++;
    return result;
}

void load_hash_form_file(FILE *file, HashTableStruct *hashTable)
{
    char *buffer, *term,*len;
    int size,i,cursor=0,termSize;
    PostingStruct data;
    len = (char*) calloc (4,1);
    hashTable->rows = (HashCellStruct *) malloc (HASH_SIZE * sizeof(HashCellStruct));
    if(feof(file)==0)
    {
        for(i=0;i<LINE_NUM; i++){
            fread(len, 1, 4, file);
            memcpy(&size,len,4);
            buffer = (char*) malloc (size * sizeof(char));
            size = fread(buffer,1, size,file);
            if(size==0)
                break;
            buffer[size-1] = '\0';
            term = get_term(buffer, &cursor);
            termSize = cursor-1;
            while(cursor<size-1){
                data.position = get_postion(buffer, &cursor);
                insert(&term, termSize, &data, hashTable);
            }
            
            free(term);
            free(buffer);
        }
        
    }

}

void merge_hash_tables_on_first(HashTableStruct* hashTable, int size)
{
    long int i,index;
    int merged = 0;
    HashCellStruct *last0, *last1;
    for(index=0;index<HASH_SIZE;index++)
        for(i=1;i<size;i++){
            if(hashTable[i].rows[index].key!=NULL) {
                if(hashTable[0].rows[index].key==NULL) { // It should completely copy to hashTable[0].rows[index]
                    hashTable[0].rows[index].key = hashTable[i].rows[index].key;
                    hashTable[0].rows[index].next = hashTable[i].rows[index].next;
                    hashTable[0].rows[index].postingList = hashTable[i].rows[index].postingList;
                }
                else{
                    if(hashTable[i].rows[index].key==NULL)
                        continue;
                    last1 = &(hashTable[i].rows[index]);
                    //printf("%s  --  %s\n",hashTable[0].rows[index].key,hashTable[i].rows[index].key );
                    while(last1!=NULL) {
                        last0 = &(hashTable[0].rows[index]);
                        merged = 0;
                        while(last0!=NULL) {
                            if(strcmp(last0->key,last1->key)==0){
                                last0->postingList->last->next = last1->postingList->first;
                                last0->postingList->last = last1->postingList->last;
                                last0->postingList->length += last1->postingList->length;
                                merged = 1;
                                break;
                            }
                            last0 = last0->next;
                        }
                        if(merged==0)
                        {
                            last0 = &(hashTable[0].rows[index]);
                            while(last0->next!=NULL)
                                last0 = last0->next;
                            last0->next = (HashCellStruct *) calloc(1, sizeof(HashCellStruct));
                            last0->next->postingList = last1->postingList;
                            last0->next->key = last1->key;
                            last0->next->next = NULL; 
                        }
                        last1 = last1->next;
                    }
                }
            }
        }
}

void merge(FILE **file, int size, char *outputFileName)
{
    int i = 0;
    HashTableStruct *hashTable;
    while(is_there_any_file_to_read(file, size)) {
        hashTable = (HashTableStruct *) malloc (size * sizeof(HashTableStruct));
        for(i=0;i<size;i++)
            load_hash_form_file(file[i],&(hashTable[i]));
        merge_hash_tables_on_first(hashTable, size);
        // write_hash(&hashTable[0], outputFileName, "a"); just for now
        for(i=0;i<size;i++)
        {
        //     //custom_free_hash_table(&hashTable[i]);
            free(hashTable[i].rows);
        }
        free(hashTable);
    }
}