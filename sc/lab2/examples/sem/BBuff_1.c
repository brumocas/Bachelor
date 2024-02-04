/*******************************
*  Producer - Consumer with a single unit buffer
*  
*  Main expects one argument - number of producer iterations
*  Producer produces consecutive number from 0 to n. of iterations - 1
*  Consumer gets those values from the buffer one at a time and prints them
*  
*  Observe the alternance between producer and consumer 
*    caused by the synchronization
*  Try with several consumers and 
*    even several producers producing different ranges
********************************/


#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

int loops;  // number of values to produce/consume
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

int buffer, count =0;  // single position buffer, initially empty

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
	int i;
	for( i=0; i<loops; i++ ){
		pthread_mutex_lock( &m );
		while( count == 0 ) 
			pthread_cond_wait( &full, &m ); // wait on fill
		int tmp = myget();   // get the value in the buffer
		pthread_cond_signal( &empty );
		pthread_mutex_unlock( &m );
		printf("Value read = %d\n", tmp);
	}

	return 0;
}

void *producer( void* arg ){
	int i;
	for( i=0; i<loops; i++ ){
		pthread_mutex_lock( &m );
		while( count == 1 ) 
			pthread_cond_wait( &empty, &m ); // wait on empty
		myput(i);   // insert i in the buffer
		pthread_cond_signal( &full );
		pthread_mutex_unlock( &m );
		printf("Value produced = %d\n", i);
	}
	return 0;
}


int main( int argc, char *argv[] ){
	assert( argc == 2 );
	loops = atoi( argv[1] );
	pthread_t p1,p2;
	
	pthread_mutex_init ( &m, NULL );
	pthread_cond_init( &full, NULL );
	pthread_cond_init( &empty, NULL );

	printf( "main: \n" );

	pthread_create( &p1, NULL, producer, NULL );
	pthread_create( &p2, NULL, consumer, NULL );
	// join: waiting for threads to finish
	pthread_join( p1, NULL );
	pthread_join( p2, NULL );

	return 0;
}
