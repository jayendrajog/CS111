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
static pthread_mutex_t lock;
volatile static int lock_m = 0;

void add(long long *pointer, long long value) {
		long long sum;
		long long  orig;
		switch(sync_option)
		{
			case 1:
				//mutex
				pthread_mutex_lock(&lock);
				sum = *pointer + value;
				 if(opt_yield)
		        	pthread_yield();
				*pointer = sum;
				pthread_mutex_unlock(&lock);
				return;
			case 2:
				//spin-lock
				while(__sync_lock_test_and_set(&lock_m,1))
				continue;
				sum = *pointer + value;
				if(opt_yield)
		        	pthread_yield();
				*pointer = sum;
				__sync_lock_release(&lock_m);
				return;
			case 3:
				do
				{
					orig = *pointer;
					sum = orig + value;
					if(opt_yield)
		        		pthread_yield();
				}
				while(__sync_val_compare_and_swap(pointer, orig, sum) != orig);
				return;
			default:
				//default
				sum = *pointer + value;
		        if(opt_yield)
		        	pthread_yield();
		        *pointer = sum;
				return;
		}
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
	long long time_difference;
	long num_operations;
	//input handling
	if(!argc)
	{
		printf("Missing input arg\n");
		return 1;
	}
	if(argc > 5)
	{
		printf("Too many input arguments\n");
		return 1;
	}

	//get parameters
	if(argc >= 2)
	{
		if(parameter_check(argv, 1)) return 1;
	}
	
	if(argc >= 3)
	{
		if(parameter_check(argv, 2)) return 1;
	}

	if(argc >= 4)
	{
		if(parameter_check(argv, 3)) return 1;
	}

	if(argc >= 5)
	{
		if(parameter_check(argv, 4)) return 1;
	}
	num_operations = 2 * num_threads * num_iters;
	
//	printf("Threads are %d\n Iterations are %d\n", num_threads, num_iters);	
	
	

	
	//notes the high resolution starting time for the run
	clock_gettime(CLOCK_MONOTONIC, &start);
	
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

	free(threads);

	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("%d threads x %d iterations x (add + subtract) = %d operations\n", num_threads, num_iters, num_operations);
	if(counter != 0)
		printf("ERROR: final count = %lld\n", counter);
	printf("elapsed time: %lld ns\n", (end.tv_sec - start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec);
	printf("per operation: %lld ns\n", ((end.tv_sec - start.tv_sec)*1000000000 + end.tv_nsec - start.tv_nsec)/num_operations);
	return 0;
}


