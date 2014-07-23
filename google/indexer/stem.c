#include "stem.h"

#include <string.h>
#include <ctype.h>
#include <stdio.h>

void stem(char* word)
{
    int i;
    int len = strlen(word);
    // Change all to lower case remove punctuations
    for(i=0; i<len; i++) {
        if(isupper(*(word+i))!=0)
            *(word+i) = tolower(*(word+i));
        if(isalnum(*(word+i)) == 0){ // Remove extra ordinary characters
                memcpy(word+i,word+i+1,len-i);
        }

    }
    for(i=0;i<strlen(word);i++){
        if(islower(*(word+i))!=0)
            break;
    }
    if(i==strlen(word))
        strcpy(word,"");
    if(strcmp(word,"the")==0 || 
        strcmp(word,"a")==0 ||
        strcmp(word,"as")==0 ||
        strcmp(word,"this")==0 ||
        strcmp(word,"that")==0 ||
        strcmp(word,"these")==0 ||
        strcmp(word,"him")==0 ||
        strcmp(word,"her")==0 ||
        strcmp(word,"i")==0 ||
        strcmp(word,"is")==0 ||
        strcmp(word,"am")==0 ||
        strcmp(word,"are")==0 ||
        strcmp(word,"were")==0 ||
        strcmp(word,"where")==0 ||
        strcmp(word,"what")==0 ||
        strcmp(word,"do")==0 ||
        strcmp(word,"does")==0 ||
        strcmp(word,"not")==0 ||
        strcmp(word,"be")==0 ||
        strcmp(word,"was")==0 ||
        strcmp(word,"of")==0 ||
        strcmp(word,"as")==0 ||
        strcmp(word,"on")==0 ||
        strcmp(word,"and")==0 ||
        strcmp(word,"or")==0 ||
        strcmp(word,"have")==0 ||
        strcmp(word,"hase")==0 ||
        strcmp(word,"had")==0 ||
        strcmp(word,"did")==0 ||
        strcmp(word,"to")==0 ||
        strcmp(word,"for")==0 ||
        strcmp(word,"from")==0)
        strcpy(word,"");

    // Check for *ing and remove ing
    len = strlen(word);
    if(len>3)
        if(strcmp(word+len-3, "ing")==0)
            bzero(word+len-3,3);

}