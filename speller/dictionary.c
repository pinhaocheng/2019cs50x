// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Declare array for hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

node *add_node(node *ptr, char *word);

// global var tracking word count
unsigned int word_count = 0;

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Allocate memory for word
    char *word = malloc(sizeof(char) * LENGTH);
    if (word == NULL)
    {
        fclose(file);
        return false;
    }

    // // Buffer for a word
    // char word[LENGTH + 1];

    // Iterate over dictionary
    while (fscanf(file, "%s", word) != EOF)
    {
        // allocating a spot of memory for this node
        node *new_node = malloc(sizeof(node));

        // check for failure allocating memory for new node
        if (!new_node)
        {
            unload();
            return false;
        }

        // Copy word into new node
        strcpy(new_node->word, word);

        // hash the given word with a value between 0 and 25
        int hash_val = hash(new_node->word);

        // Update pointer
        new_node->next = hashtable[hash_val];

        // Return pointer to new node
        hashtable[hash_val] = new_node;

        word_count++;
    }

    // Free allocated memory
    free(word);

    // Close dictionary
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // prepare array for word passed in
    char temp[LENGTH];

    // copy the word wanted to be checked into temporary holder
    strcpy(temp, word);

    // point cursor pointer to the relevant start of bucket
    node *cursor = hashtable[hash(temp)];

    // checking all the words in the basket starting from first node
    while (cursor != NULL)
    {
        // If word found return
        // strcasecmp doesn't care about capital letter or norm
        if (strcasecmp(cursor->word, temp) == 0)
        {
            return true;
        }

        // Set pointer to next element
        cursor = cursor->next;
    }

    return false;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;

}
