#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "Dtp3.h"

void *numbersProducer(void *vargp)
{
    unsigned int number;
    char buffer[BUFFER_SIZE];

    int file_descriptor = *(int *)vargp;

    while (1)
    {
        number = rand() % 1000;
        printf("Generated number %u\n", number);
        // Prepare data to be written to file (integer to fixed-size string format)
        snprintf(buffer, BUFFER_SIZE, "%09d\n", number);
        // TODO - Write to file 
        write(file_descriptor, buffer, BUFFER_SIZE - 1); //Dont print Null char

        sleep(1);
    }

    return NULL;
}

int main()
{
    int file_descriptor = -1;
    pthread_t tid_1;

    // TODO - Open file so that:
    // - only write-only operations are allowed
    // - file is created if not existing
    // - if file exists, append data (don't overwrite file)
    // - set permissions to 0644

    // Open the file to write the generated numbers to it
    file_descriptor = open(PRODUCER_FILE_NAME, O_RDWR|O_CREAT|O_APPEND, 0664);
     
    if (file_descriptor == -1)
    {
        fprintf(stderr, "Error: %s: %s\n", PRODUCER_FILE_NAME, strerror(errno));
        return -1;
    }

    // Modify the following statement if necessary (hint: it is!)
    pthread_create(&tid_1, NULL, numbersProducer, &file_descriptor);

    pthread_join(tid_1, NULL);

    close(file_descriptor);

    exit(0);
}
