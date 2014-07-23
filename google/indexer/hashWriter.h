#ifndef _HASH_WRITER_H
#define _HASH_WRITER_H

#include <stdio.h>

#include "hash.h"

// Write hash in this format: 
//   (4Byte)size[with conflict]term1#;docId1:pos1,pos2,...,TF@tf;docId2:pos1,pos2,...,TF@tf\nterm1#;docId1:pos1,pos2,...,TF@tf;docId2:pos1,pos2,...,TF@tf 
void write_hash(HashTableStruct *hashTable, char* filename, char *mode, PostingStruct *docId);


#endif