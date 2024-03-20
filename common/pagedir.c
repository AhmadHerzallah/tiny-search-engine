/*
 * Ahmad Herzallah [2024]
 * pagedir.c - handles the pagesaver mentioned in crawler directory.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../libcs50/webpage.h"

bool pagedir_init(const char *pageDirectory);
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);

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
