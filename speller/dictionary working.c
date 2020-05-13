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
        // Add word to hash table
        int hash_value = hash(word);
        hashtable[hash_value] = add_node(hashtable[hash_value], word);
        word_count++;
    }

    // Free allocated memory
    free(word);

    // Close dictionary
    fclose(file);
    return true;
}

// no problem
// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // Declare an array for word
    char term[LENGTH];

    // Copy word and convert to lowercase
    strcpy(term, word);
    for (int i = 0; word[i]; i++)
    {
        term[i] = tolower(word[i]);
    }

    // Get the hash value for word
    int hash_value = hash(term);

    // Sets list to point that `hash_value` element of table
    node *list = hashtable[hash_value];

    // Iterate over linked list
    while (list != NULL)
    {
        // If word found return
        if (strcmp(list->word, term) == 0)
        {
            return true;
        }

        // Set pointer to next element
        list = list->next;
    }

    return false;
}

// Add new node to the linked list
node *add_node(node *ptr, char *word)
{
    // Allocate memory for new node
    node *new = malloc(sizeof(node));
    if (new == NULL)
    {
        return NULL;
    }

    // Copy word into new node
    strcpy(new->word, word);

    // Update pointer
    new->next = ptr;

    // Return pointer to new node
    return new;
}

// no problem
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
