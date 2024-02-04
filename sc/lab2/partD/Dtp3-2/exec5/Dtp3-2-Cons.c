#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "Dtp3.h"

void *oddNumbersConsumer(void *vargp)
{
    int prod_file_descriptor = *(int *)vargp;
    int file_descriptor_odd;
    unsigned int number;
    char buffer[BUFFER_SIZE];
    int read_size;

    // TODO - Open the odd numbers file so that:
    // - only write-only operations are allowed
    // - file is created if not existing
    // - if file exists, overwrite existing data
    // - set permissions to 0644

    file_descriptor_odd = open(ODD_NUMBERS_FILE_NAME, O_WRONLY|O_TRUNC|O_CREAT, 0644);

    lseek(prod_file_descriptor, 0, SEEK_SET);

    if (file_descriptor_odd == -1)
    {
        fprintf(stderr, "Error: %s: %s\n", ODD_NUMBERS_FILE_NAME, strerror(errno));
        return NULL;
    }

    while (1)
    {    
        // TODO - Read one number from the producers file. Recall
        // that numbers are in text format with 9 characters.
        
        lseek(file_descriptor_odd, 0, SEEK_SET);
        while ((read_size = read(prod_file_descriptor, &buffer, BUFFER_SIZE)) > 0)
        {   // Convert text format (string) to integer
            
            if (read_size > 0)
            {   
                number = atoi(buffer);
                if (number % 2 != 0)
                {
                    printf("Storing odd number %u\n", number);
                    snprintf(buffer, BUFFER_SIZE, "%09d\n", number);

                    // TODO - Store formated odd number into the odd numbers file
                    write(file_descriptor_odd, &buffer, BUFFER_SIZE - 1);
                }
            }
               
        }
        
        usleep(250000);
    }

    close(file_descriptor_odd);

    return NULL;
}

void *evenNumbersConsumer(void *vargp)
{
    int prod_file_descriptor = *(int *)vargp;
    int file_descriptor_even;
    unsigned int number;
    char buffer[BUFFER_SIZE];
    int read_size;

    // TODO - Open the even numbers file so that:
    // - only write-only operations are allowed
    // - file is created if not existing
    // - if file exists, overwrite existing data
    // - set permissions to 0644

    // Open the file to write the generated numbers to it
    file_descriptor_even = open(EVEN_NUMBERS_FILE_NAME, O_WRONLY|O_TRUNC|O_CREAT, 0644);

    lseek(prod_file_descriptor, 0, SEEK_SET);

    if (file_descriptor_even == -1)
    {
        fprintf(stderr, "Error: %s: %s\n", EVEN_NUMBERS_FILE_NAME, strerror(errno));
        return NULL;
    }

    while (1)
    {
        // TODO - Read one number from the producers file. Recall
        // that numbers are in text format with 9 characters.
        
        lseek(file_descriptor_even, 0, SEEK_SET);
        while ((read_size = read(prod_file_descriptor, &buffer, BUFFER_SIZE)) > 0)
        {   // Convert text format (string) to integer
            
            if(read_size > 0)
            {
                number = atoi(buffer);
                if (number % 2 == 0)
                {
                    printf("Storing even number %u\n", number);
                    snprintf(buffer, BUFFER_SIZE, "%09d\n", number);
                    
                    // TODO - Store formated even number into the even numbers file
                    write(file_descriptor_even, &buffer, BUFFER_SIZE - 1);
                }
            }

        }

        usleep(250000);
    }

    close(file_descriptor_even);
    
    return NULL;
}

int main()
{
    pthread_t tid_1, tid_2;

    // TODO - Open the producer file so that:
    // - only read operations are allowed
    // - file is created if not existing
    // - set permissions to 0644
    // Hint: remember that both threads will read from the same
    // file and how file descriptors track the progress within
    // the file....

    int file_descriptor = open(PRODUCER_FILE_NAME,O_RDONLY);

    if (file_descriptor == -1)
    {
        fprintf(stderr, "Error: %s: %s\n", PRODUCER_FILE_NAME, strerror(errno));
        return -1;
    }

    pthread_create(&tid_1, NULL, oddNumbersConsumer, &file_descriptor); 
    pthread_create(&tid_2, NULL, evenNumbersConsumer, &file_descriptor);

    pthread_join(tid_1, NULL);
    pthread_join(tid_2, NULL);

    close(file_descriptor);

    exit(0);
}
