/*
 * Ahmad Herzallah [2024]
 * pagedir.c - handles the pagesaver mentioned in crawler directory.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libcs50/webpage.h"
#include "../libcs50/file.h"
#define _POSIX_SOURCE
#include <stdio.h>
#undef _POSIX_SOURCE
#include <unistd.h>

bool pagedir_init(const char *pageDirectory);
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);
webpage_t* pagedir_load(FILE* file);
bool pagedir_validatedir(const char *pageDirectory);

bool pagedir_init(const char *pageDirectory)
{
    // construct the pathname for the .crawler file in that directory
    char fileName[100];
    strcpy(fileName, pageDirectory);
    strcat(fileName, "/.crawler");

    // open the file for writing; on error, return false.
    FILE *file = fopen(fileName, "w");
    if (file == NULL)
    {
        // printf("Couldn't open file. ya basha \n");
        printf("Couldn't allocate file\n.");
        return false;
    }

    printf("File added to .crawler \n");

    // close the file and return true.
    fclose(file);

    return true;
}


void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID)
{
    // construct the pathname for the page file in pageDirectory
    char pathName[100];
    sprintf(pathName, "%s/%d", pageDirectory, docID);

    // open that file for writing
    FILE* file = fopen(pathName, "w");

    if(file)
    {
        // print the URL
        char* URL = webpage_getURL(page);
        // print the depth
        int depth = webpage_getDepth(page);

        // print the contents of the webpage
        char* content = webpage_getHTML(page);
    
        fprintf(file, "%s\n", URL);
        fprintf(file, "%d\n", depth);
        fprintf(file, "%s\n", content);
        // close the file
        fclose(file);
    } 
    else
    {
        perror("Error opening file\n");
    }
}

// module providing functions to load webpages from files in the pageDirectory;
webpage_t* pagedir_load(FILE* file)
{
    char* url = file_readLine(file);
    char* content = file_readFile(file);
    char* depth = file_readLine(file);
    int depthInt = atoi(depth);
    webpage_t* page = webpage_new(url, depthInt, content);

    if (page == NULL) {
        fprintf(stderr, "error: couldn't load new webpage from crawler-produced file, exiting non-zero\n");
        exit(1);
    }

    free(depth);

    return page;    

}

// validate it received exactly two command-line arguments and that
bool pagedir_validatedir(const char *pageDirectory) {
    char fileName[100];
    sprintf(fileName, "%s/.crawler", pageDirectory);
    // F_OK. Tests whether the file exists
    if(access(fileName, F_OK) == 0 ) {
        return true;
    }
    return false;
}
