/* Implementation of a bounded buffer with semaphores
*  Semaphores used for synchronization and mutual exclusion
*  Check the bounded buffer implementation as a circular buffer 
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <semaphore.h>

#define MAX 3  // positions in the buffer

int loops;  // number of values to produce/consume
sem_t m;  // semaphore for mutual exclusion in using buffer indexes
sem_t full, empty;  // semaphores for synchronization

int buffer[ MAX ], fill=0, use=0;  // buffer with size MAX, fill and use - indexes

void myput ( int value ){
	sem_wait( &m );
	buffer[ fill ] = value;
	fill = (fill + 1) % MAX;  //circular index, fill = 0 to MAX-1
	sem_post( &m );
}

int myget(){
	sem_wait( &m );
	int tmp = buffer[ use ];
	use = (use + 1) % MAX;  //circular index, use = 0 to MAX-1
	sem_post( &m );	
	return( tmp );
}


void *consumer( void* arg ){
	while( 1 ){
		sem_wait( &full ); // wait for buffer full
		int tmp = myget();   // get the value in the buffer
		printf("C_%lu: Value read = %d\n", pthread_self(),tmp);
		sem_post( &empty );  // post on empty semaphore
		
		//sched_yield();
	}
}

void *producer( void* arg ){
	for( int i=0; i<loops; i++ ){
		sem_wait( &empty ); // wait for buffer empty
		myput(i);   // insert i in the buffer
		printf("Value produced = %d\n", i);
		sem_post( &full );  // post on full semaphore
	//usleep( 100000 ); 
	}
		
}

int main( int argc, char *argv[] ){
	assert( argc == 2 );
	loops = atoi( argv[1] );
	pthread_t p1,p2,p3;
	
	sem_init ( &m, 0, 1 );  // semaphore for mutual exclusion on  buffer indexes
	sem_init ( &empty, 0, MAX );  // semaphore for empty buffer - MAX positions
	sem_init ( &full, 0, 0 );  // semaphore for full buffer - start with buffer empty
	printf( "main: \n" );

	pthread_create( &p1, NULL, producer, NULL );
	pthread_create( &p2, NULL, consumer, NULL );
	pthread_create( &p3, NULL, consumer, NULL );
	
	// producer finishes 
	pthread_join( p1, NULL );  // wait for producer
	sleep( 1 ); // give time for consumers to consume last values produced
	exit(0);  // finish

	return 0;
}
