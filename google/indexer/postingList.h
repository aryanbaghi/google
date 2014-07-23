#ifndef _POSTING_LIST_H
#define _POSTING_LIST_H

#include <stdlib.h>

typedef struct Posting
{
    unsigned int position;
    unsigned int docId;
    struct Posting *next;
} PostingStruct;

typedef struct PostingList 
{
    PostingStruct *first;
    PostingStruct *last; // use append function to be update
    unsigned int length; // term frequency
} PostingListStruct;

void append(PostingListStruct *postingList, PostingStruct *insert);

void free_post(PostingStruct *posting);
void free_posting_list(PostingListStruct *postingList);

#endif
