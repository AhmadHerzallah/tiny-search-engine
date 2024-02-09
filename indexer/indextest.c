/*
 * indextest.c [Feb, 2024]
 * will serve as a unit test for the index module; 
 * it reads an index file into the internal index data structure
 * then writes the index out to a new index file.
 * Ahmad Herzallah
*/


#include <stdlib.h>
#include <stdio.h>
#include "../common/index.h"


int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(stderr, "Missing Arguments or extra arguments, please make sure to only fill pageDirectory and indexFileName");
        exit(1);
    }

    char* oldIndexFilename = argv[1];
    char* newIndexFilename = argv[2];

    index_t* index = readIndex(oldIndexFilename);

    saveIndex(index, newIndexFilename);

    deleteIndex(index);

    return 0;

}