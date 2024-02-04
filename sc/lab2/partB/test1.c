#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t my_sem;

// A regular C function that executs as a thread
void *myThreadFunction(void *vargp)
{   
    	
    sem_init(&my_sem, 0, 1);
    sem_wait(&my_sem);

    return NULL;
}

int main()
{
	pthread_t tid_1, tid_2; //Thread ID
	int sem_value;

	printf("Creating 1 Threads\n");

	pthread_create(&tid_1, NULL, myThreadFunction, (void *)(&tid_1) );

	sem_getvalue(&my_sem,&sem_value);
	
	printf("Semvalue = %d \n",sem_value);

	pthread_join(tid_1, NULL);

	printf("After Running Threads\n");
	exit(0);
}
