#include <stdlib.h>
#include <string.h>
#include "SortedList.h"
#include <stdio.h>
#include <pthread.h>

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
	if (!list || !element)
		return;

	if (!list->next) {
		list->next = element;
		list->prev = element;
		element->prev = list;
		element->next = list;
		return;
	}

	SortedListElement_t *p = list;
	SortedListElement_t *n = list->next;

	while (n != list) {
		if (strcmp(element->key, n->key) <= 0)
			break;
		p = n;
		n = n->next;
	}
	
	if (opt_yield & INSERT_YIELD)
		pthread_yield();

	if (!element || !p || !n)
		return;

	element->prev = p;
	element->next = n;
	p->next = element;
	n->prev = element;
}

int SortedList_delete(SortedListElement_t *element) {
	if (!element)
		return -1;

	SortedListElement_t *n = element->next;
	SortedListElement_t *p = element->prev;
	
	if (!n || !p)
		return -1;

	if (n->prev != element)
		return 1;	// or -1??
	if (p->next != element)
		return 1;

	if (opt_yield & DELETE_YIELD)
		pthread_yield();

	if (!n || !p || !element)
		return -1;

	n->prev = p;
	p->next = n;
	element->next = NULL;
	element->prev = NULL;
	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
	if (!list || !key)
		return NULL;

	SortedListElement_t *n = list->next;
	while (n != list) {
		if (strcmp(n->key, key) == 0)
			break;
		n = n->next;
	}
	
	if (opt_yield & SEARCH_YIELD)
		pthread_yield();

	if (n != list)
		return n;
	else
		return NULL;
}

int SortedList_length(SortedList_t *list) {
	if (!list)
		return -1;

	SortedListElement_t *element = list->next;
	SortedListElement_t *n = element->next;
	SortedListElement_t *p = element->prev;
	int count = 0;

	if (!element || !n || !p)
		return -1;

	while (element != list) {
		// check for corruption
		if (n->prev != element || p->next != element)
			return -1;
		count++;
		p = element;
		element = n;
		n = element->next;	
	}

	if (opt_yield & SEARCH_YIELD)
		pthread_yield();

	return count;
}
