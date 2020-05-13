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

// Represents a hash table
node *hashtable[N];

// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';
}

// global var tracking word count
unsigned int word_count = 0;

// global var tracking if file is loaded correctly
bool loaded = NULL;

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

    // Buffer for a word
    char word[LENGTH + 1];

    // Insert words into hash table
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

        // copy the word read into new node
        strcpy(new_node->word, word);

        // set new node pointer to NULL
        new_node->next = NULL;

        // hash the given word with a value between 0 and 25
        int hash_val = hash(new_node->word);
        node *head = hashtable[hash_val];

        // check if there's previous words in the bucket
        // if there's none then set this new node to be the first
        if (head != NULL)
        {
            new_node->next = hashtable[hash_val];
        }

        hashtable[hash_val] = new_node;

        // free new_node
        free(new_node);

        // run counter
        word_count++;
    }
    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
    loaded = true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    if (loaded == true)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // prepare array for word passed in
    char temp[LENGTH];

    // copy the word wanted to be checked into temporary holder
    strcpy(temp, word);

    // convert to lower case
    for (int i = 0; word[i]; i++)
    {
        temp[i] = tolower(word[i]);
    }

    // point cursor pointer to the relevant start of bucket
    node *cursor = hashtable[hash(temp)];

    // checking all the words in the basket starting from first node
    while (cursor != NULL)
    {
        // return true when the word is found in dict
        if (strcasecmp(cursor->word, temp) == 0)
        {
            return true;
        }
        else
        {
            // if not found yet then continue to the next word
            cursor = cursor->next;
        }
    }

     return false;

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    int i = 0;

    for (i = 0; i < N; i++)
    {
        node *cursor = hashtable[i];

        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    if (i == N)
    {
        return true;
    }
    else
    {
        return false;
    }
}
