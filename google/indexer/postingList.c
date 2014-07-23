#include "postingList.h"

void append(PostingListStruct *postingList, PostingStruct *insert)
{
    (postingList->length)++;
    if(postingList->last == NULL) {
        postingList->first = (PostingStruct *) calloc(1, sizeof(PostingStruct));
        postingList->first->position = insert->position;
        postingList->first->docId = insert->docId;
        postingList->first->next = NULL;
        postingList->last = postingList->first;
    }
    else {
        postingList->last->next = (PostingStruct *) calloc(1, sizeof(PostingStruct));
        postingList->last->next->position = insert->position;
        postingList->last->next->docId = insert->docId;
        postingList->last->next->next = NULL;
        postingList->last = postingList->last->next;
    }
}
void free_post(PostingStruct *posting)
{
    PostingStruct *next = NULL;
    if(posting!=NULL)
    {
        if(posting->next!=NULL)
            next = posting->next;
        free(posting);
        // if(next!=NULL)
            // return free_post(next);
    }
}
void free_posting_list(PostingListStruct *postingList)
{
    if(postingList!=NULL)
    {
        free_post(postingList->first);
    }

}
