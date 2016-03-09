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
	int yie_count = 0;
	int yie_index;
	int yie_insert = 0;	// false
	int yie_delete = 0;
	int yie_search = 0;
	
	while ((option = getopt_long(argc, argv, "", input_options, &option_index)) != -1) {
		switch (option) {
			case THREADS:
				n_threads = strtol(argv[optind-1]+10, NULL, 10);	// 10 for length of --threads
				break;
			case ITERATIONS:
				n_iterations = strtol(argv[optind-1]+13, NULL, 10);	// 13 for length of --iterations
				break;
			case YIELD:
				// count number of arguments after --yield
				//yie_index = optind - 1;
				//while (yie_index < argc) {
				//	if (argv[yie_index][0] == '-' &&
				//}
				break;
		}
	}
	printf("Hello\n");
	printf("n_threads is %i, n_iterations is %i\n", n_threads, n_iterations);
}
