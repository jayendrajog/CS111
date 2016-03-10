#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>	// parse
#include <time.h>
#include <pthread.h>
#include "SortedList.h"

int opt_yield;

enum OPTIONS {
	THREADS,
	ITERATIONS,
	YIELD
};

typedef struct pthread_package {
	int nElements;				// number of elements to operate on (iterations)
	SortedList_t * head;			// header of the list
	SortedListElement_t ** elements;	// the address of the first element to operate on
} pthread_package_t;

void *pthread_task(void *arg) {
	pthread_package_t *myArg = (pthread_package_t *) arg;
	int i, length;
	// if we create an array we could potentially run into problem
	// it is possible that there are duplicates...and when we are looking up, it will return the same address
	// and it will create problems when deleting
	//SortedListElement_t ** foundElements = malloc(sizeof(SortedListElement_t *) * myArg->nElements);
	SortedListElement_t * foundElement;

	// insert each element into the list
	for (i = 0; i < myArg->nElements; i++)
		SortedList_insert(myArg->head, myArg->elements[i]);

	// gets the list length
	//length = SortedList_length(myArg->head);

	// look up each of the keys it inserted
	// deletes each returned element from the list
	for (i = 0; i < myArg->nElements; i++) {
		foundElement = SortedList_lookup(myArg->head, (myArg->elements[i])->key);
		SortedList_delete(foundElement);
		//free(foundElement);
		//foundElement = NULL;
	}
}

int main(int argc, char *argv[])
{
	static struct option input_options[] = {
		{"threads", required_argument, 0, THREADS},
		{"iterations", required_argument, 0, ITERATIONS},
		{"yield", required_argument, 0, YIELD},
		{0,	0,	0,	0}
	};
	
	// getopt_long varialbes
	int option;
	int option_index = 0;

	// user inputs
	int n_threads = 1;
	int n_iterations = 1;
	
	// yield option
	char c;
	opt_yield = 0;
	int i;
	//int j;
	
	// list head and elements
	int list_n_elements;
	SortedList_t *list_header;
	SortedListElement_t **list_elements;
	SortedListElement_t *list_ele;
	char list_rand_key;
	pthread_package_t * pthread_arg;
	int list_length;

	// multithreading
	struct timespec time_start, time_end;
	pthread_t *tid;	// thread ids
	void *retVal;
	int ret;
	long time_ns;
	
	while ((option = getopt_long(argc, argv, "", input_options, &option_index)) != -1) {
		switch (option) {
			case THREADS:
				n_threads = strtol(argv[optind-1]+10, NULL, 10);	// 10 for length of --threads
				break;
			case ITERATIONS:
				n_iterations = strtol(argv[optind-1]+13, NULL, 10);	// 13 for length of --iterations
				break;
			case YIELD:
				// get arguments of yield
				for (i = 8; i < 11; i++) {	// 8 for length of --yield
					if (!(c = argv[optind-1][i]))
						break;
					switch (c) {
						case 'i':
							opt_yield |= INSERT_YIELD;
							break;
						case 'd':
							opt_yield |= DELETE_YIELD;
							break;
						case 's':
							opt_yield |= SEARCH_YIELD;
							break;
						default:
							fprintf(stderr, "Unrecognized --yield argument\n");
					}
				}
				break;
			default:
				;
		}
	}
	
	// initialize empty list and elements
	list_n_elements = n_threads * n_iterations;	// number of elements in list
	list_header = malloc(sizeof(SortedList_t));	// header to list
	list_elements = malloc(sizeof(SortedListElement_t *) * list_n_elements);	// array of SortedListElement_t pointers	
	if (!list_header || !list_elements) {
		free(list_header);
		free(list_elements);
		exit(1);
	}
	list_header->prev = NULL;
	list_header->next = NULL;
	list_header->key = NULL;

	srand(time(NULL));	// initialize random seed
	for (i = 0; i < list_n_elements; i++) {
		list_ele = malloc(sizeof(SortedListElement_t));	// TODO: check list_ele != NULL
		if (!list_ele)	// TODO: cleanup
			exit(1);
		list_rand_key = (char) rand();	// TODO: doesn't seem that random (all k's and K's)
		list_ele->key = &list_rand_key;	// TODO: I'm not sure if we need to malloc for key
		list_elements[i] = list_ele;
	}

	// start the timer	
	clock_gettime(CLOCK_REALTIME, &time_start);

	tid = malloc(sizeof(pthread_t) * n_threads);
	if (!tid)	// TODO: cleanup
		exit(1);

	for (i = 0; i < n_threads; i++) {
		pthread_arg = malloc(sizeof(pthread_package_t));	// TODO: check malloc
		if (!pthread_arg)	// TODO: cleanup
			exit(1);
		pthread_arg->nElements = n_iterations;
		pthread_arg->head = list_header;
		pthread_arg->elements = &(list_elements[i*n_iterations]);	// where it should start
		ret = pthread_create(&tid[i], NULL, pthread_task, (void *) pthread_arg);
		// TODO: error checking on ret
	}

	for (i = 0; i < n_threads; i++) {
		ret = pthread_join(tid[i], (void**) retVal);
		// TODO: error checking on ret
	}

	clock_gettime(CLOCK_REALTIME, &time_end);

	time_ns = (time_end.tv_sec - time_start.tv_sec) * 1000000000 + (time_end.tv_nsec - time_start.tv_nsec);

	if (list_header) {
		list_length = SortedList_length(list_header);
		if (list_length)
			fprintf(stderr, "List length is not 0! It's %d\n", list_length);
	}
	
	//printf("End of main\n");
	printf("elapsed time: %ld ns\n", time_ns);

	// cleanup
	for (i = 0; i < list_n_elements; i++) {
		//free(list_elements[i]);
		//list_elements[i] = NULL;
	}
}
