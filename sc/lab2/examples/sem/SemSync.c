/* Synchronization with semaphores
*  One thread reads characters from the keyboard
*  Whenever it detects a special character (defined as SP_CHAR)
*  It posts to a semaphore on which another processing thread is blocked
*  This post will unblock the processing thread that writes to the terminal
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define SP_CHAR 'Z' // special char

sem_t s;  // semaphore used for synchronization


void *reader( void* arg ){
	printf("reader ready: Type characters on the keyboard\n");
	while(1){
		char ch = getchar();  // read from keyboard

		if( ch == SP_CHAR )
			sem_post( &s ); // post to the semaphore
	}
}


void *handler( void* arg ){
	printf("handler ready: waiting for special char\n");
	while(1){
		sem_wait( &s );  // wait until semaphore opens
		printf("\nA '%c' was spotted\n", SP_CHAR );
	}
}


int main( int argc, char *argv[] ){
	pthread_t p1,p2;
	printf( "main: \n" );
	sem_init( &s, 0, -2 ); //Zero value means that this is for thread handling
	
	pthread_create( &p1, NULL, reader, NULL );
	pthread_create( &p2, NULL, handler, NULL );
	
	// join: waiting for threads to finish � will never do
	pthread_join( p1, NULL );
	pthread_join( p2, NULL );

	return 0;
}
