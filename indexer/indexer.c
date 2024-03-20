/*
 * indexer.c [Feb, 2024]
 * reads the document files produced by the TSE crawler,
 * builds an index, and writes that index to a file.
 * Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.
 * Ahmad Herzallah
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../common/index.h"
#include "../common/word.h"
#include "../libcs50/bag.h"
#include "../libcs50/file.h"
#include "../libcs50/hash.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/set.h"
#include "../libcs50/webpage.h"

//   creates a new 'index' object
//  loops over document ID numbers, counting from 1
//    loads a webpage from the document file 'pageDirectory/id'
//    if successful, 
//      passes the webpage and docID to indexPage

webpage_t* pagedir_load(FILE* file);
bool pagedir_validatedir(const char *pageDirectory);


void indexPage(webpage_t* webpage, int docID, index_t* index)
{
    int pos = 0;
    char* res;

    // steps through each word of the webpage
    while ((res = webpage_getNextWord(webpage, &pos)) != NULL) {

        // skips trivial words (less than length 3),
        if (strlen(res) >= 3) {
            // normalizes the word (converts to lower case),
            char* normalizedWord = normalizeWord(res);
            // looks up the word in the index,
            counters_t* wordCounter = getIndex(index, normalizedWord);
            if (wordCounter == NULL) {
                // adding the word to the index if needed
                addIndex(index, normalizedWord);
                // increments the count of occurrences of this word in this docID
                counters_add(getIndex(index, normalizedWord), docID);
            }
            else {
                // increments the count of occurrences of this word in this docID
                counters_add(wordCounter, docID);
            }
            free(normalizedWord);
        }
        free(res);
    }

}

void indexBuild(const char* pageDirectory, const char* indexFilename) 
{
    // check if file exists.
    if (!pagedir_validatedir(pageDirectory)) {
        fprintf(stderr, "Coudln't find targeted file\n");
        exit(1);
    }

    FILE* file;
    int docID = 1;
    char fileName[100];
    
    // creates a new 'index' object
    index_t* index = newIndex(NULL);
    sprintf(fileName, "%s/%d", pageDirectory, docID);

    // loops over document ID numbers, counting from 1
    while ((file = fopen(fileName, "r")) != NULL) {
        // loads a webpage from the document file 'pageDirectory/id'
        
        webpage_t* webpage = pagedir_load(file);
        if (webpage)
        {
            // if successful, 
            // passes the webpage and docID to indexPage
            indexPage(webpage, docID, index);
            webpage_delete(webpage);
        }
        docID += 1;
        sprintf(fileName, "%s/%d", pageDirectory, docID);
        fclose(file);
    }
    saveIndex(index, indexFilename);
    deleteIndex(index);
}


// steps through each word of the webpage,
//   skips trivial words (less than length 3),
//   normalizes the word (converts to lower case),
//   looks up the word in the index,
//     adding the word to the index if needed
//   increments the count of occurrences of this word in this docID



// parse the command line, validate parameters, initialize other modules
// call indexBuild, with pageDirectory

int indexer(int argc, char *argv[]){
    char* pageDirectory = argv[1];
    char* indexFilename = argv[2];

    indexBuild(pageDirectory, indexFilename);

    return 0;

}


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Missing Arguments or extra arguments, please make sure to only fill pageDirectory and indexFileName");
        exit(1);
    }
    
    indexer(argc, argv);
}

