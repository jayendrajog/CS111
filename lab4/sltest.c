#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>	// parse
#include "SortedList.h"

enum OPTIONS {
	THREADS,
	ITERATIONS,
	YIELD
};

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
	printf("Hello\n");
	printf("n_threads is %i, n_iterations is %i\n", n_threads, n_iterations);
}
