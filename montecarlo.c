#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <math.h>

double pi_sum = 0.0;
double pi_sum2 = 0.0;
int total_N = 0;

#define MAX 100
pthread_t thread_id[MAX];
pthread_mutex_t mutexsum = PTHREAD_MUTEX_INITIALIZER;
 
void* calc_pi(void * n);

int main(int argc, char* argv[]){
	int i, n, numthreads = 0;
	double error, relative_error, final_pi = 0.0;

	srand( time(NULL) ); /* seeded the random number generator outside of the thread function
				so it doesn't generate same random number in all threads */

	if(argc != 3){
		fprintf(stderr, "Not enough arguments!\n"), exit(EXIT_FAILURE);
	}

	/* checks if there number of threads passed into command line is 0 - 100 and the N value is valid */ 
	if ((numthreads = atoi(argv[1])) < 0 || numthreads > 99){
		fprintf(stderr, "Invalid number of threads\n"), exit(EXIT_FAILURE);
	}
	if ((n = atoi(argv[2])) <= 0){
		fprintf(stderr, "Invalid N value"), exit(EXIT_FAILURE);
	}
	
	for(i = 0; i < numthreads; i++){
		if(pthread_create(&thread_id[i], NULL, calc_pi, (void *) n) > 0){
			fprintf(stderr, "An error occurred while creating threads\n"), exit(EXIT_FAILURE);
		}
	}

	for(i = 0; i < numthreads; i++){
		if(pthread_join(thread_id[i], NULL) > 0){
			fprintf(stderr, "Error joining thread\n"), exit(EXIT_FAILURE);
		}
	}

	/* calculate our estimate of pi, statistical error, relative error and then print it out to stdout */
	final_pi = pi_sum/numthreads;
	error = sqrt(((pi_sum2/numthreads) - final_pi * final_pi)/(numthreads - 1.0));
	relative_error = (final_pi - M_PI)/M_PI; 

	printf("The final pi is %f\n The statistical error is %f\n The relative error is %f\n The total number of points are %i\n", final_pi, error, relative_error, total_N);

	exit(EXIT_SUCCESS);	
}

void* calc_pi(void * n){
	int i, thread_sum = 0;
	double x, y, ans, thread_pi;
	int N = (int)n;

	/* for each thread, calculate x^2 + y^2, n times. If the result is >= 1, then we add that to the local variable
	thread_sum and calculate an estimate of pi for each thread */

	for(i = 0; i < N; i++){
		x = (double)rand()/(double)RAND_MAX;
		y = (double)rand()/(double)RAND_MAX;

		if((ans = x*x + y*y) <= 1){
			thread_sum += 1;
		}
	}

	/* must cast num_times to double or else division will be int */ 
	thread_pi = 4.0 * (thread_sum/(double)N);	

	/* the following needs mutex lock because all threads will have access to this global variable also */
	pthread_mutex_lock(&mutexsum);
	pi_sum += thread_pi;
	pi_sum2 += thread_pi * thread_pi;
	total_N += N; 
	pthread_mutex_unlock(&mutexsum);

	pthread_exit((void *) 0);
}
