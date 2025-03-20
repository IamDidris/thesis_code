



#ifndef ALGORITHM_SET_H
#define ALGORITHM_SET_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _Set Set;


typedef struct _SetIterator SetIterator;


typedef struct _SetEntry SetEntry;

#ifdef TEST_ALTERNATE_VALUE_TYPES
#include "alt-value-type.h"
#else


typedef void *SetValue;


#define SET_NULL ((void *) 0)

#endif 


struct _SetIterator {
	Set *set;
	SetEntry *next_entry;
	unsigned int next_chain;
};


typedef unsigned int (*SetHashFunc)(SetValue value);


typedef int (*SetEqualFunc)(SetValue value1, SetValue value2);


typedef void (*SetFreeFunc)(SetValue value);


Set *set_new(SetHashFunc hash_func, SetEqualFunc equal_func);


void set_free(Set *set);


void set_register_free_function(Set *set, SetFreeFunc free_func);


int set_insert(Set *set, SetValue data);


int set_remove(Set *set, SetValue data);


int set_query(Set *set, SetValue data);


unsigned int set_num_entries(Set *set);


SetValue *set_to_array(Set *set);


Set *set_union(Set *set1, Set *set2);


Set *set_intersection(Set *set1, Set *set2);


void set_iterate(Set *set, SetIterator *iter);


int set_iter_has_more(SetIterator *iterator);


SetValue set_iter_next(SetIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif 
