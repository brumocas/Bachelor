#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// A regular C function that executs as a thread
void *myThreadFunction(void *vargp)
{   
    sem_t my_sem;
	int sem_value;
    sem_init(&my_sem, 0, 10);
	for (int i = 0; i < 6; i++)
    sem_wait(&my_sem);
	for (int i = 0; i < 4; i++)
	sem_post(&my_sem);
	sem_getvalue(&my_sem,&sem_value);
    printf("Semvalue = %d \n",sem_value);

    return NULL;
}

int main()
{
	pthread_t tid_1, tid_2; //Thread ID

	printf("Creating 1 Threads\n");

	pthread_create(&tid_1, NULL, myThreadFunction, (void *)(&tid_1) );

	pthread_join(tid_1, NULL);

	printf("After Running Threads\n");
	exit(0);
}
