/*
 * index.h [Feb 2024]
 * a module providing the data structure to represent
 * the in-memory index, and functions to read and write index files
 * Ahmad Herzallah
*/

#include <stdlib.h>
#include "../libcs50/counters.h"

typedef struct index index_t;

// counters pairs for document id
void idCounter(void* arg, const int key, const int count);

// counters pairs for document word
void wordCounter(void* arg, const char* key, void* item);

// creates a new index hashtable
index_t* newIndex(int* wordsCount);

// adds word into index
bool addIndex(index_t* index, char* word);

// return index based on a word
counters_t* getIndex(index_t* index, char* word);

// iterate index ht and return it to a file
index_t* readIndex(char* fileName);

// Save index hashtable to a file
void saveIndex(index_t* index, const char* indexFilename);

// Delete index hashtable
void deleteIndex(index_t* index);