/* Implementation of a bounded buffer (size 1) with semaphores
*  Semaphores used for synchronization and mutual exclusion
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>

int loops;  // number of values to produce/consume

int buffer, count = 0;  // single position buffer, initially empty
sem_t empty, full;  // semaphores for buffer empty and full conditions

void myput ( int value ){
	assert( count == 0 );
	count = 1;
	buffer = value;
}

int myget(){
	assert( count == 1 );
	count = 0;
	return( buffer );
}


void *consumer( void* arg ){
	for( int i=0; i<loops; i++ ){
		sem_wait( &full ); // wait for buffer full
		int tmp = myget();   // get the value in the buffer
		printf("Value read = %d\n", tmp);
		sem_post( &empty );  // post on empty semaphore
		
	}
}

void *producer( void* arg ){
	for( int i=0; i<loops; i++ ){
		sem_wait( &empty ); // wait for empty buffer
		myput(i);   // insert i in the buffer
		printf("Value produced = %d\n", i);
		sem_post( &full );  // post on full semaphore
		
	}
}


int main( int argc, char *argv[] ){
	assert( argc == 2 );
	loops = atoi( argv[1] );
	pthread_t p1,p2;
	
	sem_init ( &empty, 0, 1 );
	sem_init ( &full, 0, 0 );
	printf( "main: \n" );

	pthread_create( &p1, NULL, producer, NULL );
	pthread_create( &p2, NULL, consumer, NULL );
	// join: waiting for threads to finish
	pthread_join( p1, NULL );
	pthread_join( p2, NULL );
}
