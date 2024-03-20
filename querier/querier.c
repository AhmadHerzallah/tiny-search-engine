/*
 * querier.c [Feb, 2024]
 * program that reads the index file produced by the TSE Indexer,
 * and page files produced by the TSE Querier,
 * and answers search queries submitted via stdin.
 * Ahmad Herzallah
 */

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../common/index.h"
#include "../common/pagedir.h"
#include "../common/word.h"
#include "../libcs50/bag.h"
#include "../libcs50/counters.h"
#include "../libcs50/file.h"
#include "../libcs50/hash.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/mem.h"
#include "../libcs50/set.h"
#include "../libcs50/webpage.h"


// Function prototypes
void updateCount(void *arg, const int key, const int count);
int countItemsInCounters(counters_t *counters);
void initializeOrUpdateIntersectionCount(void *arg, const int key, const int count);
void updateIntersectionCounters(void *arg, const int key, const int count);
counters_t *computeCountersIntersection(counters_t *firstCounters, counters_t *secondCounters);
void updateUnion(void *arg, const int key, const int count);
int tokQuery(char *input, char *words[]);
counters_t *unionCounters(counters_t *firstCounter, counters_t *secondCounter);
counters_t *normalizeQuery(char *input, index_t *index);
char *grabURL(int docID, char *pageDirectory);
void updateMaxNodeValue(void *arg, const int key, const int count);
void printSearchResults(counters_t *result, char *pageDirectory);
int querier(int argc, char *argv[]);
bool isLogicalOperator(char *word);

struct countersNode
{
	int key;
	int count;
};

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		fprintf(stderr, "Usage: querier: pageDirectory indexFilename\n");
		exit(1);
	}
	else if (argc > 3)
    {
        fprintf(stderr, "Error: Too many arguments provided.\n");
        fprintf(stderr, "Usage: querier: pageDirectory indexFilename\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++)
    {
        if (argv[i][0] == '\0')
        {
            fprintf(stderr, "Error: Argument %d is an empty string.\n", i);
			fprintf(stderr, "Usage: querier: pageDirectory indexFilename\n");
            exit(1);
        }
    }

	querier(argc, argv);
	return 0;
}

// Function definitions...

// Check if a word is an operator (either "and" or "or")
bool isLogicalOperator(char *word)
{
	return (strcmp(word, "and") == 0 || strcmp(word, "or") == 0);
}

void mergeCounters(counters_t *destinationCounters, counters_t *sourceCounters)
{
    if (destinationCounters == NULL || sourceCounters == NULL) {
        // fprintf(stderr, "Error: NULL pointer passed to mergeCounters.\n");
        return;
    }

	counters_iterate(sourceCounters, destinationCounters, &updateUnion);
}

// Update counter for counting non-zero key-value pairs
void updateCount(void *arg, const int key, const int count)
{
	int *itemCount = arg;
	if (key != 0 && count != 0)
	{
		(*itemCount)++;
	}
}

// Get the size of a counters object
int countItemsInCounters(counters_t *counters)
{
    if (counters == NULL) {
        // fprintf(stderr, "Error: NULL pointer passed to countItemsInCounters.\n");
        return -1; // Return an error value or handle it according to your application's logic
    }

	int itemCount = 0;
	counters_iterate(counters, &itemCount, &updateCount);
	return itemCount;
}

// Prepare counters for intersection operation
void initializeOrUpdateIntersectionCount(void *arg, const int key, const int count)
{
    if (arg == NULL) {
        // fprintf(stderr, "Error: NULL pointer passed to initializeOrUpdateIntersectionCount.\n");
        return;
    }

    // Cast the argument pointer to the appropriate data structure type
	counters_t *iteratingCounters = arg;

    // Check if the key exists in the counters
	if (counters_get(iteratingCounters, key) == 0)
	{
        // If the key does not exist, initialize it with the provided count
		counters_set(iteratingCounters, key, 0);
	}
}

// Update counters for intersection operation
void updateIntersectionCounters(void *arg, const int key, const int count)
{
    // Cast the argument pointer to the appropriate data structure type
	counters_t *intersectionCounters = arg;

    // Get the count from the intersection counters
	int firstCount = counters_get(intersectionCounters, key);

    // Update the intersection counters if the key exists
	if (firstCount != 0)
	{
        // Calculate the minimum count between the existing count and the new count
		int min = (firstCount <= count) ? firstCount : count;
		counters_set(intersectionCounters, key, min);
	}
}

// Perform intersection operation on two counters
counters_t *computeCountersIntersection(counters_t *firstCounters, counters_t *secondCounters)
{
	// Determine which counters to use as the base for intersection
	counters_t *intersectionCounters =
	    (countItemsInCounters(firstCounters) <= countItemsInCounters(secondCounters)) ? firstCounters : secondCounters;
	counters_t *iteratingCounters = (intersectionCounters == firstCounters) ? secondCounters : firstCounters;

    // Initialize intersection counters with the counts from iterating counters
	counters_iterate(intersectionCounters, iteratingCounters, &initializeOrUpdateIntersectionCount);
    // Update intersection counters with minimum counts from iterating counters
	counters_iterate(iteratingCounters, intersectionCounters, &updateIntersectionCounters);

	return intersectionCounters;
}

// Update counters for union operation
void updateUnion(void *arg, const int key, const int count)
{
	counters_t *unionCounters = arg;
	int currentCount = counters_get(unionCounters, key);
	counters_set(unionCounters, key, currentCount + count);
}

// Tokenize query into words
int tokQuery(char *input, char *words[])
{
	int wordCount = 0;
	char *word = input;

	while (*word != '\0')
	{
		while (*word != '\0' && !isalpha(*word))
		{
			word++;
		}

		if (*word == '\0')
		{
			break;
		}

		char *end = word;
		while (*end != '\0' && isalpha(*end))
		{
			end++;
		}

		size_t wordLength = end - word;
		char *normalizedWord = malloc((wordLength + 1) * sizeof(char));
		if (normalizedWord == NULL)
		{
			fprintf(stderr, "Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}

		strncpy(normalizedWord, word, wordLength);
		normalizedWord[wordLength] = '\0';

		words[wordCount] = normalizedWord;
		wordCount++;

		word = end;
	}

	return wordCount;
}

// Union two counters
counters_t *unionCounters(counters_t *firstCounter, counters_t *secondCounter)
{
	counters_t *unionCounters = counters_new();

	counters_iterate(firstCounter, unionCounters, &updateUnion);
	counters_iterate(secondCounter, unionCounters, &updateUnion);

	return unionCounters;
}

// Normalize the query and perform intersection and union operations
counters_t *normalizeQuery(char *input, index_t *index)
{
	if (input == NULL || input[0] == '\0')
	{
		printf("Please enter a word to query.");
		return NULL;
	}
	
	// Validate input characters
	for (int i = 0; i < strlen(input); i++)
	{
		if (!isalpha(input[i]) && !isspace(input[i]))
		{
			fprintf(stderr, "Error: invalid character '%c', taking next query\n", input[i]);
			return NULL;
		}
	}

	// Tokenize input query
	int arrayLength = strlen(input) / 2;
	char *words[arrayLength];
	int wordCount = tokQuery(input, words);

	printf("Query: ");
	for (int i = 0; i < wordCount; i++)
	{
		printf("%s ", words[i]);
	}
	printf("\n");

	// Check if the first and last words are not operators
	if (isLogicalOperator(words[0]))
	{
		fprintf(stderr, "Error: first word is an operator, taking next query\n");
		return NULL;
	}
	else if (isLogicalOperator(words[wordCount - 1]))
	{
		fprintf(stderr, "Error: last word is an operator, taking next query\n");
		return NULL;
	}

	// Check for adjacent operators
	for (int i = 0; i < wordCount; i++)
	{
		if (isLogicalOperator(words[i]))
		{
			if (isLogicalOperator(words[i + 1]))
			{
				fprintf(stderr, "Error: adjacent operators, taking next query\n");
				return NULL;
			}
		}
	}

	// Initialize counters for intersection and union operations
	counters_t *countersUnion = counters_new();
	counters_t *countersIntersection = getIndex(index, words[0]);

	// Perform union and intersection operations
	for (int i = 1; i < wordCount; i++)
	{
		if (strcmp(words[i], "or") == 0)
		{
			mergeCounters(countersUnion, countersIntersection);
			countersIntersection = counters_new();
			countersIntersection = getIndex(index, words[i + 1]);
			i++;
		}
		else if (!isLogicalOperator(words[i]))
		{
			countersIntersection = computeCountersIntersection(countersIntersection, getIndex(index, words[i]));
		}
	}

	mergeCounters(countersUnion, countersIntersection);

	// Free memory allocated for words
	for (int i = 0; i < wordCount; i++)
	{
		free(words[i]);
	}

	return countersUnion;
}

// Retrieve URL for a given docID
char *grabURL(int docID, char *pageDirectory)
{
	char fileName[100];
	sprintf(fileName, "%s/%d", pageDirectory, docID);

	FILE *file = fopen(fileName, "r");
	if (file != NULL)
	{
		char *URL = file_readLine(file);
		fclose(file);
		return URL;
	}
	else
	{
		fprintf(stderr, "Error: failed to retrieve docID URL, exiting non-zero\n");
		exit(1);
	}
}

// Update the maximum node
void updateMaxNodeValue(void *arg, const int key, const int count)
{
	if (arg != NULL)
	{
		struct countersNode *maxNodePointer = arg;
		if (count > maxNodePointer->count)
		{
			maxNodePointer->count = count;
			maxNodePointer->key = key;
		}
	}
}

// Print search results
void printSearchResults(counters_t *result, char *pageDirectory)
{
	if (result == NULL)
	{
		printf("Error: Null counterset pointer.\n");
		return;
	}

	int itemCount = 0;
	counters_iterate(result, &itemCount, &updateCount);
	if (itemCount == 0)
	{
		printf("No documents match.\n");
	}
	else
	{
		struct countersNode maxNode = {0, 0};
		struct countersNode *maxNodePointer = &maxNode;

		counters_iterate(result, maxNodePointer, &updateMaxNodeValue);
		char *URL = grabURL(maxNodePointer->key, pageDirectory);
		printf("Matches %d document(s) (ranked):\n", itemCount);
		printf("docID: %d, score: %d, URL: %s", maxNodePointer->key, maxNodePointer->count, URL);
		free(URL);
		printf("\n");
		counters_set(result, maxNodePointer->key, 0);
		while (maxNodePointer->count != 0)
		{
			maxNodePointer->count = 0;
			counters_iterate(result, maxNodePointer, &updateMaxNodeValue);
			if (maxNodePointer->count != 0)
			{
				char *URL = grabURL(maxNodePointer->key, pageDirectory);
				printf("docID: %d, score: %d, URL: %s", maxNodePointer->key, maxNodePointer->count, URL);
				free(URL);
				printf("\n");
				counters_set(result, maxNodePointer->key, 0);
			}
		}
	}
	printf("-------------------------------------------");
	printf("\n");
	counters_delete(result); // Delete the counterset only after using it
}

// Main function for the querier
int querier(int argc, char *argv[])
{
	char *pageDirectory = argv[1];
	char *indexFilename = argv[2];

	char input[256];

	if (!pagedir_validatedir(pageDirectory))
	{
		printf("Error: This file doesn't exist bruh.\n");
		return false;
	}
	
    if (access(indexFilename, F_OK) != 0)
    {
        fprintf(stderr, "Error: The specified index file does not exist.\n");
        return 1;
    }

	FILE *fp = fopen(indexFilename, "r");
	if (fp == NULL)
	{
		fclose(fp);
		fprintf(stderr, "Error: exiting non-zero\n");
		exit(1);
	}
	fclose(fp);

	// Process queries until end of input
	while (1)
	{
		printf("Query? ");
		if (fgets(input, sizeof(input), stdin) == NULL)
		{
			if (feof(stdin))
			{
				break;
			}
			else
			{
				printf("Error: reading input.\n");
				continue;
			}
		}

		// Read the index file
		index_t *index = readIndex(indexFilename);
		if (index == NULL)
		{
			fprintf(stderr, "Error: Failed to read index file\n");
			return 1;
		}

		// Normalize and process the query
		counters_t *counters = normalizeQuery(input, index);
		if (counters != NULL)
		{
			printSearchResults(counters, pageDirectory);
		}

		deleteIndex(index);
	}

	return 0;
}
