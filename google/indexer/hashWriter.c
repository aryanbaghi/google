#include "hashWriter.h"

extern const unsigned long long  HASH_SIZE;


char* get_text_of_hash_cell(HashCellStruct *cell, PostingStruct *docId,unsigned int *length)
{
    int i=0,size=0,j;
    unsigned int docIdIndex = 0;
    unsigned int termFreq = 0;
    PostingStruct *docIdBackup=docId;
    char *result;
    PostingStruct *postingCursor;
    HashCellStruct *temp;
    temp=cell;
    while(temp!=NULL){
        i+=temp->postingList->length * sizeof(unsigned long long int)*5; //for posting list
        i+=temp->postingList->length; //for posting list's comma and \n
        i+=strlen(temp->key)+1; // for key
        i++; //for \n
        temp=temp->next;
    } 
    result = (char*) calloc(i*2+1,1);
    i=0;
    temp=cell;
    while(temp!=NULL) {
        docIdIndex = 0;
        docId = docIdBackup;
        termFreq = 0;
        strcpy(result+i,temp->key);
        i+=strlen(temp->key);
        result[i] = '#';
        i++;
        // if(*length>10)
            // printf("%d-%s-\n", *length,temp->key);
        postingCursor = temp->postingList->first;
        result[i]=';';
        while(postingCursor != NULL){
            // size=sprintf(result+i,"%d,",postingCursor->position);
            ///////
            while(docId->position < postingCursor->position && docId->next!=NULL)
            {
                if(docId->next->position > postingCursor->position)
                    break;
                if(result[i]!=';')
                {
                    size = sprintf(result+i, "TF@%u", termFreq);
                    i+=size;
                    termFreq = 0;
                    result[i]=';';
                }
                docId = docId->next;
                docIdIndex++;
            }
            if(result[i]==';'){
                i++;
                j = sprintf(result+i, "%d", docIdIndex);
                i+=j;
                result[i++]=':';
            }
            // memcpy(result+i,&postingCursor->position,sizeof(postingCursor->position));
            // result[i+sizeof(&postingCursor->position)+1]=',';
            // size = sizeof(&postingCursor->position)+1;
            size = sprintf(result+i, "%u,", postingCursor->position);
            ////////
            i+=size;
            postingCursor = postingCursor->next;
            termFreq++;
        }
        size = sprintf(result+i, "TF@%u", termFreq);
        i+=size;
        result[i] = '\n';
        i++;
        temp = temp->next;
    }
    *length = i;
    return result;
}

void write_hash(HashTableStruct *hashTable, char* filename , char *mode, PostingStruct *docId)
{
    FILE *file;
    char *text;
    char INT[4];
    PostingStruct *docIdCursor=docId;
    unsigned long long int i = 0;
    unsigned int length;
    file = fopen(filename, mode);
    while(docIdCursor!=NULL){
        i++;
        docIdCursor = docIdCursor->next;
    }
    length = (i+1)*sizeof(unsigned long long int) * 5;
    text = (char*) calloc(length,sizeof(char));
    docIdCursor = docId;
    i=0;
    while(docIdCursor!=NULL){
        i+=sprintf(text+i,"%u,",docIdCursor->position);
        docIdCursor = docIdCursor->next;
    }
    printf("here");
    length = strlen(text);
    memcpy(INT,&length,4);
    fwrite(INT, 1, 4, file);
    fwrite(text, 1, length, file);
    free(text);
    length = 0;
    for(i = 0; i<HASH_SIZE ;i++) {
        if(hashTable->rows[i].key!=NULL)
        {
            length = 0;
            text = get_text_of_hash_cell(&hashTable->rows[i],docId,&length);
            memcpy(INT,&length, 4);
            fwrite(INT, 1, 4, file);
            fwrite(text, 1, length, file);
            free(text);
            // printf("%lu : %d - %s\n",i, hashTable->rows[i].postingList->first->position, hashTable->rows[i].key);
        }
    }
    fclose(file);
}

