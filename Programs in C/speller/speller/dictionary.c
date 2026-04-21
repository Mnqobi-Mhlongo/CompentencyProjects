// Implements a dictionary's functionality
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// Initialize word_count
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Takes index and establishes pointer
    int index = hash(word);
    node *pointer = table[index];

    // Loops through the dictionary, if present returns true
    while (pointer != NULL)
    {
        if (strcasecmp(pointer->word, word) == 0)
        {
            return true;
        }
        else
        {
            node *next = pointer->next;
            pointer = next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    unsigned int h = 0;

    for (int i = 0; word[i] != '\0'; i++)
    {
        h = (h * 23 + toupper(word[i])) % N;
    }

    return h;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{

    // TODO
    // Open files
    FILE *source = fopen(dictionary, "r");
    char word[LENGTH + 1];
    if (source == NULL)
    {
        printf("Dictionary not found %s.\n", dictionary);
        return false;
    }

    // Loops to take work while fscanf does not return EOF
    while (fscanf(source, "%s", word) != EOF)
    {
        // Allocate node for number
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            fclose(source);
            return false;
        }

        // Prepend node to list and copies with strcpy
        int index = hash(word);
        strcpy(n->word, word);
        n->next = table[index];
        table[index] = n;

        // Increments word count
        word_count++;
    }

    // Close files
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    for (int i = 0; i < N ; i++)
    {
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *next = cursor->next;
            free(cursor);
            cursor = next;
        }
    }
    return true;
}
