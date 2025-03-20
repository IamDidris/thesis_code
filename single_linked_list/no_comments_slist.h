



#ifndef ALGORITHM_SLIST_H
#define ALGORITHM_SLIST_H

#ifdef __cplusplus
extern "C" {
#endif


typedef struct _SListEntry SListEntry;


typedef struct _SListIterator SListIterator;

#ifdef TEST_ALTERNATE_VALUE_TYPES
#include "alt-value-type.h"
#else


typedef void *SListValue;


#define SLIST_NULL ((void *) 0)

#endif 


struct _SListIterator {
	SListEntry **prev_next;
	SListEntry *current;
};


typedef int (*SListCompareFunc)(SListValue value1, SListValue value2);


typedef int (*SListEqualFunc)(SListValue value1, SListValue value2);


void slist_free(SListEntry *list);


SListEntry *slist_prepend(SListEntry **list, SListValue data);


SListEntry *slist_append(SListEntry **list, SListValue data);


SListEntry *slist_next(SListEntry *listentry);


SListValue slist_data(SListEntry *listentry);


void slist_set_data(SListEntry *listentry, SListValue value);


SListEntry *slist_nth_entry(SListEntry *list, unsigned int n);


SListValue slist_nth_data(SListEntry *list, unsigned int n);


unsigned int slist_length(SListEntry *list);


SListValue *slist_to_array(SListEntry *list);


int slist_remove_entry(SListEntry **list, SListEntry *entry);


unsigned int slist_remove_data(SListEntry **list, SListEqualFunc callback,
                               SListValue data);


void slist_sort(SListEntry **list, SListCompareFunc compare_func);


SListEntry *slist_find_data(SListEntry *list, SListEqualFunc callback,
                            SListValue data);


void slist_iterate(SListEntry **list, SListIterator *iter);


int slist_iter_has_more(SListIterator *iterator);


SListValue slist_iter_next(SListIterator *iterator);


void slist_iter_remove(SListIterator *iterator);

#ifdef __cplusplus
}
#endif

#endif 
