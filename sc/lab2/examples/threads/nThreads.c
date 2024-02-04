#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define T 3  // number of threads to create
typedef void *(thr_fun_t)(void *arg);  // define a pointer to a function

/* try making this variable global, local static, and local dynamic
*  don't forget (un)commenting as needed
*/
int example_var;

// A regular C function that executs as a thread
void *threadFunction(void *vargp)
{
	//static int example_var;
	//int example_var;
    int id = *(int*)vargp;
	printf("Starting thread %d \n", id );
	example_var = id;
	sleep(id);
	printf("thread %d: example_var=%d\n", id, example_var );
	//_exit(0);   // uncomment to see the whole set of threads ending ---> KILL the process
	pthread_exit(NULL);  // two equivalent ways of ending a thread
	//return NULL;
}

int main()
{
	pthread_attr_t attr[T];
	pthread_t tid[T];
	int thr_arg[T]={1,2,3};

	printf("Creating %d threads\n", T);

	for(int i = 0; i < T; i++ ) {
		pthread_attr_init(&attr[i]); /* Initialize attr */ 
		pthread_create(&(tid[i]), &(attr[i]),
			(thr_fun_t *) threadFunction,
			(void *)&(thr_arg[i]));
	}

	for(int i = 0; i < T; i++ ) {
		pthread_join(tid[i], NULL);
	}

    
	printf("All threads ended\n");
	exit(0);
}
