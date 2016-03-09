#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>	// parse
#include <time.h>
#include <pthread.h>
#include "SortedList.h"

enum OPTIONS {
	THREADS,
	ITERATIONS,
	YIELD
};

typedef struct pthread_package {
	SortedList_t * head;
	SortedListElement_t ** elements;
} pthread_package_t;

void *pthread_task(void *arg) {
	printf("I am a thread\n");
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
	int yield_insert = 0;	// false
	int yield_delete = 0;
	int yield_search = 0;
	char c;

	int i;
	//int j;
	
	// list head and elements
	int list_n_elements;
	SortedList_t *list_header;
	SortedListElement_t **list_elements;
	SortedListElement_t *list_ele;
	char list_rand_key;
	pthread_package_t * pthread_arg;

	// multithreading
	struct timespec time_start, time_end;
	pthread_t *tid;	// thread ids
	void *retVal;
	int ret;
	
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
							yield_insert = 1;
							break;
						case 'd':
							yield_delete = 1;
							break;
						case 's':
							yield_search = 1;
							break;
						default:
							fprintf(stderr, "Unrecognized --yield argument\n");
					}
				}
				break;
			default:
				fprintf(stderr, "CRITICAL ERROR! SHOULD NEVER GET HERE!\n");
		}
	}
	//printf("Hello\n");
	//printf("n_threads is %i, n_iterations is %i\n", n_threads, n_iterations);
	
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
		list_rand_key = (char) rand();
		list_ele->key = &list_rand_key;	// TODO: I'm not sure if we need to malloc for key
		list_elements[i] = list_ele;
	}

	// prepare pthread package argument (structure) to be used for helper function
	//pthread_arg.head = list_header;
	//pthread_arg.elements = list_elements;
	
	// start the timer
	clock_gettime(CLOCK_REALTIME, &time_start);

	tid = malloc(sizeof(pthread_t) * n_threads);
	for (i = 0; i < n_threads; i++) {
		pthread_arg = malloc(sizeof(pthread_package_t));	// TODO: check malloc
		pthread_arg->head = list_header;
		pthread_arg->elements = list_elements;
		ret = pthread_create(&tid[i], NULL, pthread_task, (void *) pthread_arg);
		// TODO: error checking on ret
	}

	for (i = 0; i < n_threads; i++) {
		ret = pthread_join(tid[i], (void**) retVal);
		// TODO: error checking on ret
	}

	clock_gettime(CLOCK_REALTIME, &time_end);

	printf("End of main\n");
}
