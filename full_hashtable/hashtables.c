#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair *new_pair = create_pair(key, value); // make a new pair instance
  int index = hash(key, ht->capacity);            // get the index to place it at

  if (ht->storage[index] == NULL) // if nothing is there, store new pair
  {
    ht->storage[index] = new_pair;
  }
  else if (strcmp(ht->storage[index]->key, key) == 0)
  {
    // if something exists in index and has the same key, replaced
    ht->storage[index] = new_pair;
  }
  else
  {
    // if pair exists in index and is different, add new pair as the next thing in the list
    ht->storage[index]->next = new_pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *old_pair = NULL;

  while (current_pair != NULL)
  {
    if (strcmp(current_pair->key, key) == 0)
    {
      if (old_pair != NULL)
      {
        old_pair->next = current_pair->next; // removes current pair from the linked list
      }
      else // if there is no old pair, it is the beginning of the linked list
      {
        ht->storage[index] = current_pair->next; // therefore, just assign index the next thing in the list
      }

      destroy_pair(current_pair);
    }
    else
    {
      old_pair = current_pair;
      current_pair = current_pair->next;
    }
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[index];

  while (current_pair != NULL)
  {
    if (strcmp(current_pair->key, key) == 0)
    {
      return current_pair->value;
    }
    else
    {
      current_pair = current_pair->next;
    }
  }
  return NULL;
}
/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  if (ht->storage != NULL)
  {
    for (int i = 0; i < ht->capacity; i++)
    {
      LinkedPair *current_pair = ht->storage[i];
      while (current_pair != NULL)
      {
        LinkedPair *old_pair = current_pair;
        current_pair = current_pair->next;
        destroy_pair(old_pair);
      }
    }
  }

  if (ht != NULL)
  {
    free(ht);
  }
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  for (int i = 0; i < ht->capacity; i++)
  {
    LinkedPair *current_pair = ht->storage[i];

    while (current_pair != NULL)
    {
      hash_table_insert(new_ht, current_pair->key, current_pair->value);
      current_pair = current_pair->next;
    }
  }

  destroy_hash_table(ht);

  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  // hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  // printf("%s", hash_table_retrieve(ht, "line_1"));
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  ht = hash_table_resize(ht);
  printf("%s\n", hash_table_retrieve(ht, "line-1"));

  destroy_hash_table(ht);

  return 0;
}
#endif
