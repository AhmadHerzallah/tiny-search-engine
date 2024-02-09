/*
 * index.c [Feb 2024]
 * a module providing the data structure to represent
 * the in-memory index, and functions to read and write index files
 * Ahmad Herzallah
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../libcs50/hashtable.h"
#include "../libcs50/hash.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"

typedef struct index {
    hashtable_t* ht;
} index_t;

// counters pairs for document id
void idCounter(void* arg, const int key, const int count) {
    FILE* file = (FILE*) arg;
    if (key >= 0 && count > 0) {
        fprintf(file, "%d %d ", key, count);
    }
}

// counters pairs for document word
void wordCounter(void* arg, const char* key, void* item) {
    FILE* file = (FILE*) arg;
    fprintf(file, "%s ", key);
    if (key != NULL && item != NULL) {
        counters_iterate(item, arg, &idCounter);
    }
    fprintf(file, "\n");
}

// creates a new index hashtable
index_t* newIndex(int* wordsCount) {
    index_t* newIndex = malloc(sizeof(index_t*));
    if (newIndex == NULL) {
        fprintf(stderr, "Couldn't allocate memory\n");
        exit(1);
    }    
    if (wordsCount != NULL) {
        newIndex->ht = hashtable_new(*wordsCount);
    }
    else {
        newIndex->ht = hashtable_new(500);
    }
    return newIndex;
}

// adds word into index
bool addIndex(index_t* index, char* word) {
    counters_t* counters = counters_new();
    bool flag = hashtable_insert(index->ht, word, counters);
    return flag;
}

// return index based on a word
counters_t* getIndex(index_t* index, char* word) {
    counters_t* item = hashtable_find(index->ht, word);
    return item;
}

// iterate index ht and return it to a file
index_t* readIndex(char* fileName) {
    char* word;
    int docID = 0;
    int count = 0;

    FILE* file = fopen(fileName, "r");
    if (file == NULL) {
        fprintf(stderr, "Couldn't open %s \n", fileName);
        exit(1);
    }

    int numLines = file_numLines(file);
    int* numLinesPointer = &numLines;
    index_t* index = newIndex(numLinesPointer);

    while ((word = file_readWord(file)) != NULL) {
        addIndex(index, word);
        counters_t* counters = getIndex(index, word);
        while ((fscanf(file, "%d %d", &docID, &count)) == 2) {
            counters_add(counters, docID);
            counters_set(counters, docID, count);
        }
        free(word);
    }

    fclose(file);

    return index;
}


// Save index hashtable to a file
void saveIndex(index_t* index, const char* indexFilename) {
    FILE* file = fopen(indexFilename, "w");
    if (file == NULL) {
        fprintf(stderr, "Couldn't find file\n");
        exit(1);
    }

    hashtable_iterate(index->ht, file, &wordCounter);

    fclose(file);
}

// Delete index hashtable
void deleteIndex(index_t* index) {
    hashtable_delete(index->ht, (void (*)(void *))counters_delete);
    free(index);
}

