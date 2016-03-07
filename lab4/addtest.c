#include <stdio.h>
#include <time.h> //clock_gettime
#include <pthread.h> //threads

void add(void * input)
{
	long long sum = *(input.pointer) + input.value;
	*(input.pointer) = sum;
}

typedef struct
{
	long long *counter;
	long long value;
} 
input;

int main(int argc, char *argv[])
{
	//default values
	int num_threads = 1;
	int num_iters = 1;
	long long *counter = malloc(sizeof(long long));	
	struct timespec start; //for clock_gettime
	//input handling
	if(!argc)
	{
		printf("Missing input arg\n");
		return 1;
	}
	if(argc > 3)
	{
		printf("Too many input arguments\n");
		return 1;
	}

	//get parameters
	if(argc >= 2)
	{
		if(check_for_thread(argv[1]))
			num_threads = getval(argv[1]);
		if(check_for_iter(argv[1]))
			num_iters = getval(argv[1]);	
	}
	
	if(argc == 3)
	{
		if(check_for_thread(argv[2]))
			num_threads = getval(argv[2]);
		if(check_for_iter(argv[2]))
			num_iters = getval(argv[2]);
	}

//	printf("Threads are %d\n Iterations are %d\n", num_threads, num_iters);	
	
	//initializes a counter to 0
	*counter = 0;
	
	//notes the high resolution starting time for the run
	clock_gettime(CLOCK_MONOTONIC, &start);
	
	//start threads
	pthread_t * threads = malloc(nthreads * sizeof(pthread_t));
	input * inputs = malloc(nthreads * sizeof(input));
	int thread_index = 0;
	int error_check;
	for(; thread_index < num_threads; thread_index++)
	{
		input[thread_index].counter = counter;
		input[thread_index].value = 1;
		error_check = pthread_create(&threads[thread_index], NULL, add, (void*) &input[thread_index]);
		if(error_check)
		{
			printf("Error in creating thread\n");
			return 1;
		}
	}
	for(thread_input = 0; thread_input < num_threads; thread_input++)
	{
		pthread_join(threads[thread_input], NULL);
	}
	
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
