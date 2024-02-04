/* Mutual exclusion with a mutex
*  Several threads (MAX_THRD) increment a global counter cooperatively
*  The global counter is protected with a mutex
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THRD 5

int max;  // limit to the counting by each thread
int nThrd;  // number of counting threads to create
volatile int cnt = 0;  // a counter to be incremented 
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *counterThrd( void *arg ){
	int id = (int)arg;  // argument is a pointer to an integer 
	int local_cnt;
	
	printf("T%d: beginning\n", id);
	for (int i = 0; i < max; i++) {
		pthread_mutex_lock(&lock);
		local_cnt = cnt;
		local_cnt++;
		usleep(100);
		cnt = local_cnt;
		//cnt = cnt + 1; // shared: only one
		pthread_mutex_unlock(&lock);
		sched_yield();  // forces a context switch
	}
	printf( "T%d: done\n", id );
	return NULL;
}

int main( int argc, char *argv[] ){
	pthread_t pn[MAX_THRD];
	char str[5];  // letter that identifies each thread

	if( argc !=3 ){
		printf("usage: %s max_count n_threads\n", argv[0]);
		exit(-1);
	}
	max = atoi( argv[1] );
	nThrd = atoi( argv[2] );
	
	pthread_mutex_init( &lock, NULL );
	
	printf( "main: begin [cnt = %d] [%p]\n", cnt, &cnt );

	for( int i=0; i<nThrd; i++)
		pthread_create( &pn[i], NULL, counterThrd, (void*)i );

	// join: waiting for the threads to finish
	for( int i=0; i<nThrd; i++)
		pthread_join( pn[i], NULL );

	printf( "main: done\n [cnt: %d]\n [expect: %d]\n",cnt, max*nThrd );
	return 0;
}
