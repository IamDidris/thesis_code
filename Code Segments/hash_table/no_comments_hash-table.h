



#ifndef ALGORITHM_HASH_TABLE_H
#define ALGORITHM_HASH_TABLE_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _HashTable HashTable;


typedef struct _HashTableIterator HashTableIterator;


typedef struct _HashTableEntry HashTableEntry;

#ifdef TEST_ALTERNATE_VALUE_TYPES
#include "alt-value-type.h"
#else


typedef void *HashTableKey;


typedef void *HashTableValue;


#define HASH_TABLE_KEY_NULL ((void *) 0)


#define HASH_TABLE_NULL ((void *) 0)

#endif 


typedef struct _HashTablePair {
	HashTableKey key;
	HashTableValue value;
} HashTablePair;


struct _HashTableIterator {
	HashTable *hash_table;
	HashTableEntry *next_entry;
	unsigned int next_chain;
};


typedef unsigned int (*HashTableHashFunc)(HashTableKey value);


typedef int (*HashTableEqualFunc)(HashTableKey value1, HashTableKey value2);


typedef void (*HashTableKeyFreeFunc)(HashTableKey value);


typedef void (*HashTableValueFreeFunc)(HashTableValue value);


HashTable *hash_table_new(HashTableHashFunc hash_func,
                          HashTableEqualFunc equal_func);


void hash_table_free(HashTable *hash_table);


void hash_table_register_free_functions(HashTable *hash_table,
                                        HashTableKeyFreeFunc key_free_func,
                                        HashTableValueFreeFunc value_free_func);


int hash_table_insert(HashTable *hash_table, HashTableKey key,
                      HashTableValue value);


HashTableValue hash_table_lookup(HashTable *hash_table, HashTableKey key);


int hash_table_remove(HashTable *hash_table, HashTableKey key);


unsigned int hash_table_num_entries(HashTable *hash_table);


void hash_table_iterate(HashTable *hash_table, HashTableIterator *iter);


int hash_table_iter_has_more(HashTableIterator *iterator);


HashTablePair hash_table_iter_next(HashTableIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif 
