/***********************
* Producer - Consumer problem using threads and conditional variables
*
*  One thread reads characters from the keyboard and writes on a buffer
*  Another threads reads from buffer and writes on the terminal
*  Characters are written/read on/from the buffer one at a time
*  Synchronization is assured with two conditional variables (full and empty)
*
*  Exercise: Try changing this code for a buffer with size N>1
*  
************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

#define END_CHAR 'Z' // special char to end the process

char buffer[1]; // single position buffer 
int count =0;  //initially empty

pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;


void myput ( char ch ){
	assert( count == 0 );
	count = 1;
	buffer[0] = ch;
}

char myget(){
	assert( count == 1 );
	count = 0;
	return( buffer[0] );
}



void *consumer( void* arg ){

	while( 1 ){
		pthread_mutex_lock( &m );
		while( count == 0 ) 
			pthread_cond_wait( &full, &m ); // wait on full
		char  ch = myget();   // get the char  in the buffer
		pthread_cond_signal( &empty );
		pthread_mutex_unlock( &m );
		putchar( ch );
		//usleep( 100000 ); // slow writing to terminal
	}
}


void *producer( void* arg ){

	while( 1 ){
		pthread_mutex_lock( &m );
		while( count == 1 ) 
			pthread_cond_wait( &empty, &m ); // wait on empty
			char ch = getchar();
			if( ch == END_CHAR )
				exit( 0 );
			myput( ch );   // insert char ch in the buffer
		pthread_cond_signal( &full );
		pthread_mutex_unlock( &m );
	}
}


int main( int argc, char *argv[] ){
	pthread_t p1,p2,p3,p4;

	pthread_mutex_init ( &m, NULL );
	printf( "main:  threads started\n" );
	// count = 5;  // uncomment to see what "assert" does
	
	pthread_create( &p1, NULL, producer, NULL );
	pthread_create( &p3, NULL, producer, NULL );
	pthread_create( &p2, NULL, consumer, NULL );
	pthread_create( &p4, NULL, consumer, NULL );
	// join: waiting for threads to finish
	pthread_join( p1, NULL );
	pthread_join( p2, NULL );

	return 0;
}

