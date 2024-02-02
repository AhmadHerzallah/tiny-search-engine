/*
 * Ahmad Herzallah [2024]
 * crawler.c - program that crawls the web and retrieves webpages starting from a "seed" URL.
 * It parses the seed webpage, extracts any embedded URLs, then retrieves each of those pages,
 * recursively, but limiting its exploration to a given "depth".
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include "../common/pagedir.h"
#include "../libcs50/bag.h"
#include "../libcs50/file.h"
#include "../libcs50/hash.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/set.h"
#include "../libcs50/webpage.h"

// Given arguments from the command line,
// extract them into the function parameters;
// return only if successful.
bool pagedir_init(const char *pageDirectory);
void pagedir_save(const webpage_t *page, const char *pageDirectory, const int docID);

void parseArgs(const int argc, char *argv[], char **seedURL, char **pageDirectory, int *maxDepth)
{
    char *normalizedURL = normalizeURL(*seedURL);

    if (!isInternalURL(normalizedURL))
    {
        fprintf(stderr, "URL is not normalized. \n");
        exit(1);
    }
    free(normalizedURL); // Changed mem_free to free

    if (!pagedir_init(*pageDirectory))
    {
        exit(1);
    }

    if (*maxDepth < 0 || *maxDepth > 10)
    {
        fprintf(stderr, "Depth is out of range. max depth. \n");
        exit(1);
    }

    return;
}

// loops over pages to explore, until the list is exhausted

// which extracts URLs from a page and processes each one
// parse the webpage to extract all its embedded URLs

void pagescanner(webpage_t *page, bag_t *bag, hashtable_t *ht, int counter)
{
    int id = 0;
    char *data;

    while ((data = webpage_getNextURL(page, &id)) != NULL)
    {

        // printf("found: %s\n", result);
        int currentDepth = webpage_getDepth(page);
        char *normalizedURL = normalizeURL(data);
        char *memoryAllocatedNormalizerURL = malloc(strlen(normalizedURL) + 1);
        printf("%d Found: %s \n", counter, normalizedURL);
        if (memoryAllocatedNormalizerURL == NULL)
        {
            fprintf(stderr, "error: couldn't allocate memory for URL, exiting non-zero \n");
            exit(1);
        }
        else
        {
            strcpy(memoryAllocatedNormalizerURL, normalizedURL);
            if (isInternalURL(normalizedURL))
            {
                printf("%d Fetched: %s \n", counter, normalizedURL);
                bool flag = hashtable_insert(ht, normalizedURL, "");
                
                if (flag)
                {
                    if (hashtable_find(ht, normalizedURL))
                    {
                        printf("%d Scanning: %s \n", counter, normalizedURL);
                        webpage_t *newPage = webpage_new(memoryAllocatedNormalizerURL, currentDepth + 1, NULL);
                        bag_insert(bag, newPage);
                    } else {
                        printf("%d IgnDupl: %s \n",counter, normalizedURL);
                    }
                    
                }
                else
                {
                    free(memoryAllocatedNormalizerURL);
                }
            }
            else
            {
                printf("%d IgnExtrn: %s \n", counter, normalizedURL);
                free(memoryAllocatedNormalizerURL);
            }
            free(normalizedURL);
            free(data);
        }
    }
    printf("\n");
}

void crawler(char *seedURL, char *pageDirectory, const int maxDepth)
{
    int counter = 0;
    printf("Crawling >:D \n");

    hashtable_t *ht = hashtable_new(5);
    bag_t *bag = bag_new();
    // add seedURL to the bag of webpages to crawl, marked with depth=0
    webpage_t *page = webpage_new(seedURL, 0, NULL);

    hashtable_insert(ht, seedURL, "");
    bag_insert(bag, page);

    int docID = 1;
    // extract a webpage (URL,depth) item from the bag
    webpage_t *webPage = bag_extract(bag);
    // while there are more webpages in the bag:
    while (webPage != NULL)
    {
        sleep(1); // pause for one second
        // pagefetcher to retrieve a webpage for that URL
        if (webpage_fetch(webPage))
        {
            // write the webpage to the pageDirectory with a unique document ID
            pagedir_save(webPage, pageDirectory, docID);
            docID++;
            // if the webpage depth is < maxDepth
            if (webpage_getDepth(webPage) < maxDepth)
            {
                // parse the webpage to extract all its embedded URLs
                // explore the webpage to find the links it contains:
                pagescanner(webPage, bag, ht, counter);
            }
        }
        webpage_delete(webPage);
        // Corrected argument here:
        // webPage = (webpage_t *)bag_extract(bag); // Cast the return value to webpage_t*
        webPage = bag_extract(bag);
        counter++;
    }

    hashtable_delete(ht, NULL);
    bag_delete(bag, webpage_delete);
}
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Missing arguments, please make sure to include: seedURL, pageDirectory, and maxDepth\n");
        exit(1);
    }

    char *seedURL = mem_malloc(strlen(argv[1]) + 1);
    if (seedURL == NULL)
    {
        fprintf(stderr, "Couldn't reserve memory for seedURL\n");
        exit(1);
    }
    strcpy(seedURL, argv[1]);

    char *pageDirectory = argv[2];
    int maxDepth = atoi(argv[3]);

    parseArgs(argc, argv, &seedURL, &pageDirectory, &maxDepth);
    crawler(seedURL, pageDirectory, maxDepth);

    return 0;
}
