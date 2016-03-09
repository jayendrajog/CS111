#include <stdlib.h>
#include <string.h>
#include "SortedList.h"

void SortedList_insert(SortedList_t *list, SortedListElement_t *element) {
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
	
	element->prev = p;
	element->next = n;
	p->next = element;
	n->prev = element;
}

int SortedList_delete(SortedListElement_t *element) {
	SortedListElement_t *n = element->next;
	SortedListElement_t *p = element->prev;

	if (n->prev != element)
		return 1;	// or -1??
	if (p->next != element)
		return 1;

	n->prev = p;
	p->next = n;
	element->next = NULL;
	element->prev = NULL;
	free(element);
	return 0;
}

SortedListElement_t *SortedList_lookup(SortedList_t *list, const char *key) {
	SortedListElement_t *n = list->next;
	while (n != list) {
		if (strcmp(n->key, key) == 0)
			break;
		n = n->next;
	}
	
	if (n != list)
		return n;
	else
		return NULL;
}

int SortedList_length(SortedList_t *list) {
	SortedListElement_t *element = list->next;
	SortedListElement_t *n = element->next;
	SortedListElement_t *p = element->prev;
	int count = 0;

	while (element != list) {
		// check for corruption
		if (n->prev != element || p->next != element)
			return -1;
		count++;
		p = element;
		element = n;
		n = element->next;	
	}
	
	return count;
}
