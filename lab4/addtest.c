#include <stdio.h>
#include <time.h> //clock_gettime
#include <pthread.h> //threads
#include <stdlib.h>

#define _GNU_SOURCE //for pthread_yield
#define P_MUTEX 1
#define S_LOCK 2
#define S_ATOMIC 3

//initializes a counter to 0
static long long counter = 0;
static int num_iters = 1;
static int num_threads = 1;
static int sync_option = 0;
int opt_yield = 0;

void add(long long *pointer, long long value) {
        long long sum = *pointer + value;
        if(opt_yield)
        	pthread_yield();
        *pointer = sum;
    }

void* fwrapper(void* arg)
{
	int i; int j;
	for( i = 0; i < num_iters; i++)
		add(&counter, 1);
	for( j = 0; j < num_iters; j++)
		add(&counter, -1);
}


int getval(char* arg)
{
	int index = 1;
	//iterate to the '='
	while(arg[index] != '\0' && arg[index-1] != '=')
	{
		index++;
	}
	//normal case

	if(arg[index] != '\0')
	{
		return atoi(arg + index);
	}
	//error case
	return -1;
}

int check_for_iter(char * arg)
{
	//format should be '--iter'
	//returns true for --iter and --iterations
	return (arg[2] == 'i' && arg[3] == 't' && arg[4] == 'e' && arg[5] == 'r');
}

int check_for_thread(char * arg)
{
	//format should be '--threads'
	return (arg[2] == 't' && arg[3] == 'h' && arg[4] == 'r' && arg[5] == 'e' && arg[6] == 'a' && arg[7] == 'd' && arg[8] == 's');
}

int check_for_yield(char * arg)
{
	//format should be '--threads'
	return (arg[2] == 'y' && arg[3] == 'i' && arg[4] == 'e' && arg[5] == 'l' && arg[6] == 'd');
}

int check_for_sync(char * arg)
{
	//format should be '--sync'
	return (arg[2] == 's' && arg[3] == 'y' && arg[4] == 'n' && arg[5] == 'c');
}

int parameter_check(char* argv[], int pos)
{

		if(check_for_thread(argv[pos]))
			num_threads = getval(argv[pos]);
		if(check_for_iter(argv[pos]))
			num_iters = getval(argv[pos]);
		if(check_for_yield(argv[pos]))
		{
			if(getval(argv[pos]) == 0)
				opt_yield = 0;
			if(getval(argv[pos]) == 1)
				opt_yield = 1;
			else 
			{
				printf("ERROR: invalid input for yield");
				return 1;
			}

		}
		if(check_for_sync(argv[pos]))
		{
			int index = 1;
			//iterate to the '='
			while(argv[pos][index] != '\0' && argv[pos][index-1] != '=')
			{
				index++;
			}
			if(argv[pos][index] != '\0')
			{
				if(argv[pos][index] == 'm')
					sync_option = P_MUTEX;
				else if (argv[pos][index] == 's')
					sync_option = S_LOCK;
				else if (argv[pos][index] == 'c')
					sync_option = S_ATOMIC;
				else
					printf("ERROR: invalid input for sync");
			}
			else 
			{
				printf("ERROR: invalid input for sync");
				return 1;
			}
		}	
		return 0;
}

int main(int argc, char *argv[])
{
	
	struct timespec start, end; //for clock_gettime
	long time_difference;
	long num_operations;
	//input handling
	if(!argc)
	{
		printf("Missing input arg\n");
		return 1;
	}
	if(argc > 4)
	{
		printf("Too many input arguments\n");
		return 1;
	}

	//get parameters
	if(argc >= 2)
	{
		if(parameter_check(argv, 1)) return 1;
	// 	if(check_for_thread(argv[1]))
	// 		num_threads = getval(argv[1]);
	// 	if(check_for_iter(argv[1]))
	// 		num_iters = getval(argv[1]);
	// 	if(check_for_yield(argv[1]))
	// 	{
	// 		if(getval(argv[1]) == 0)
	// 			opt_yield = 0;
	// 		if(getval(argv[1]) == 1)
	// 			opt_yield = 1;
	// 		else 
	// 		{
	// 			printf("ERROR: invalid input for yield");
	// 			return 1;
	// 		}

	// 	}
	// 	if(check_for_sync(argv[1]))
	// 	{
	// 		int index = 1;
	// 		//iterate to the '='
	// 		while(argv[1][index] != '\0' && argv[1][index-1] != '=')
	// 		{
	// 			index++;
	// 		}
	// 		if(argv[1][index] != '\0')
	// 		{
	// 			if(argv[1][index] == 'm')
	// 				sync_option = P_MUTEX;
	// 			else if (argv[1][index] == 's')
	// 				sync_option = S_LOCK;
	// 			else if (argv[1][index] == 'c')
	// 				sync_option = S_ATOMIC;
	// 			else
	// 				printf("ERROR: invalid input for sync");
	// 		}
	// 		else 
	// 		{
	// 			printf("ERROR: invalid input for sync");
	// 			return 1;
	// 		}
	// 	}	
	}
	
	if(argc >= 3)
	{
		if(parameter_check(argv, 2)) return 1;
		// if(check_for_thread(argv[2]))
		// 	num_threads = getval(argv[2]);
		// if(check_for_iter(argv[2]))
		// 	num_iters = getval(argv[2]);
		// if(check_for_yield(argv[2]))
		// {
		// 	if(getval(argv[2]) == 0)
		// 		opt_yield = 0;
		// 	if(getval(argv[2]) == 1)
		// 		opt_yield = 1;
		// 	else 
		// 	{
		// 		printf("ERROR: invalid input for yield");
		// 		return 1;
		// 	}

		// }	
	}

	if(argc == 4)
	{
		if(parameter_check(argv, 3)) return 1;
		// if(check_for_thread(argv[3]))
		// 	num_threads = getval(argv[3]);
		// if(check_for_iter(argv[3]))
		// 	num_iters = getval(argv[3]);
		// if(check_for_yield(argv[3]))
		// {
		// 	if(getval(argv[3]) == 0)
		// 		opt_yield = 0;
		// 	if(getval(argv[3]) == 1)
		// 		opt_yield = 1;
		// 	else 
		// 	{
		// 		printf("ERROR: invalid input for yield");
		// 		return 1;
		// 	}

		// }	
	}
	num_operations = 2 * num_threads * num_iters;
	
//	printf("Threads are %d\n Iterations are %d\n", num_threads, num_iters);	
	
	

	
	//notes the high resolution starting time for the run
	clock_gettime(CLOCK_REALTIME, &start);
	
	//start threads
	pthread_t * threads = malloc(num_threads * sizeof(pthread_t));
	
	int thread_index = 0;
	int error_check;
	for(; thread_index < num_threads; thread_index++)
	{
		error_check = pthread_create(&threads[thread_index], NULL, fwrapper, NULL);
		if(error_check)
		{
			printf("Error in creating thread\n");
			return 1;
		}
	}
	for(thread_index = 0; thread_index < num_threads; thread_index++)
	{
		pthread_join(threads[thread_index], NULL);
	}
	clock_gettime(CLOCK_REALTIME, &end);
	time_difference = end.tv_nsec - start.tv_nsec;
	printf("%d threads x %d iterations x (add + subtract) = %d operations\n", num_threads, num_iters, num_operations);
	if(counter != 0)
		printf("ERROR: final count = %d\n", counter);
	printf("elapsed time: %d ns\n", time_difference);
	printf("per operation: %d ns\n", time_difference/num_operations);
	return 0;
}


