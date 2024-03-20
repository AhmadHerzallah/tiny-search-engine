/*
 * word.c [Feb 2024]
 * a module providing a function to normalize a word
 * Ahmad Herzallah
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
char* normalizeWord(char* word)
{
    char* res = calloc(strlen(word) + 1, sizeof(char));

    for (size_t i = 0; i < strlen(word); i++)
    {
        res[i] = tolower((unsigned char)word[i]);
    }
    return res;
    
}