



#ifndef ALGORITHM_TRIE_H
#define ALGORITHM_TRIE_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Trie Trie;

#ifdef TEST_ALTERNATE_VALUE_TYPES
#include "alt-value-type.h"
#else


typedef void *TrieValue;


#define TRIE_NULL ((void *) 0)

#endif 


Trie *trie_new(void);


void trie_free(Trie *trie);


int trie_insert(Trie *trie, char *key, TrieValue value);


int trie_insert_binary(Trie *trie, unsigned char *key, int key_length,
                       TrieValue value);


TrieValue trie_lookup(Trie *trie, char *key);


TrieValue trie_lookup_binary(Trie *trie, unsigned char *key, int key_length);


int trie_remove(Trie *trie, char *key);


int trie_remove_binary(Trie *trie, unsigned char *key, int key_length);


unsigned int trie_num_entries(Trie *trie);

#ifdef __cplusplus
}
#endif

#endif 
