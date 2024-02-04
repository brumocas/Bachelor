/* Synchronization with conditional variables
*  One thread reads characters from the keyboard
*  Whenever it detects a special character (defined as SP_CHAR)
*  It sets a conditions variable that awakes another thread (processing)
*  which writes a message to the terminal
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SP_CHAR 'Z' // special char

char ch; // last character read from keyboard
pthread_cond_t c = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;


void *reader( void* arg ){
	printf("reader ready: Type characters on the keyboard\n");
	while(1){

		pthread_mutex_lock( &m );
		ch = getchar();
		if( ch == SP_CHAR )
			pthread_cond_signal( &c ); // signal 
		pthread_mutex_unlock( &m );

		sched_yield();   // forces a context switch
		usleep(1*1000);  // at least 1ms to get a new char and signal if needed
	}
}


void *handler( void* arg ){
	printf("handler ready: waiting for special char\n");
	while(1){
		pthread_mutex_lock( &m );
		pthread_cond_wait( &c, &m );  // wait until signal
		printf("\nA '%c' was spotted\n", SP_CHAR );
		pthread_mutex_unlock( &m );
	}
}


int main( int argc, char *argv[] ){
	pthread_t p1,p2;
	printf( "main: \n" );
	pthread_mutex_init( &m, NULL );
	
	pthread_create( &p1, NULL, reader, NULL );
	pthread_create( &p2, NULL, handler, NULL );
	
	// join: waiting for threads to finish � will never do
	pthread_join( p1, NULL );
	pthread_join( p2, NULL );

}
