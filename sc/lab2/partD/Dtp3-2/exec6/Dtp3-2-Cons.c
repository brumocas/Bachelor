    #include <stdio.h>
    #include <stdlib.h>
    #include <fcntl.h>
    #include <unistd.h>
    #include <pthread.h>
    #include <string.h>
    #include <errno.h>

    #define PRODUCER_FILE_NAME "numbers.txt"
    #define ODD_NUMBERS_FILE_NAME "odd_numbers.txt"
    #define EVEN_NUMBERS_FILE_NAME "even_numbers.txt"
    #define BUFFER_SIZE 11



    struct flock lock; // lock is a file lock (type flock)


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

        file_descriptor_odd = open(ODD_NUMBERS_FILE_NAME, O_WRONLY|O_TRUNC|O_CREAT, 0664);

        
        if (file_descriptor_odd == -1)
        {
            fprintf(stderr, "Error: %s: %s\n", ODD_NUMBERS_FILE_NAME, strerror(errno));
            return NULL;
        }

        while (1)
        {    
            // TODO - Read one number from the producers file. Recall
            // that numbers are in text format with 9 characters.

            // specify the type of lock, for reading F_RDLCK or for writing F_WRLCK
            lock.l_type = F_RDLCK; // specify lock for reading
            // try acquiring the lock and, if busy, wait until lock is free (F_SETLKW)
            if( fcntl( prod_file_descriptor, F_SETLKW, &lock ) < 0 )
                exit(-1); // error in acquiring the lock
            // lock was acquired, you can read from the file here
            
            lseek(prod_file_descriptor, 0, SEEK_SET);
            lseek(file_descriptor_odd, 0, SEEK_SET);
            
            while (read( prod_file_descriptor, buffer, BUFFER_SIZE ) > 0)
            {   
                //buffer[BUFFER_SIZE - 1]= '\0';
                // Convert text format (string) to integer
                number = atoi(buffer);
                if (number % 2 != 0)
                {
                    printf("Storing odd number %u\n", number);
                    snprintf(buffer, BUFFER_SIZE , "%09d\n", number);

                    // TODO - Store formated odd number into the odd numbers file
                    write( file_descriptor_odd, buffer, BUFFER_SIZE - 1);
            
                }
                
            }
            
            
            // When done, you can release the lock explicitly
            lock.l_type = F_UNLCK;
            if (fcntl( prod_file_descriptor, F_SETLK, &lock) < 0) // no need to wait, command F_SETLK
                exit(-1); // error in releasing the lock
            // this lock is released
            usleep(2500000);
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


        if (file_descriptor_even == -1)
        {
            fprintf(stderr, "Error: %s: %s\n", EVEN_NUMBERS_FILE_NAME, strerror(errno));
            return NULL;
        }

        while (1)
        {
            // TODO - Read one number from the producers file. Recall
            // that numbers are in text format with 9 characters.


            // specify the type of lock, for reading F_RDLCK or for writing F_WRLCK
            lock.l_type = F_RDLCK; // specify lock for reading
            // try acquiring the lock and, if busy, wait until lock is free (F_SETLKW)
            if( fcntl( prod_file_descriptor, F_SETLK, &lock ) < 0 )
                exit(-1); // error in acquiring the lock
            // lock was acquired, you can read from the file here

            lseek(prod_file_descriptor, 0, SEEK_SET);
            lseek(file_descriptor_even, 0, SEEK_SET);

            while (read(prod_file_descriptor, buffer, BUFFER_SIZE  ) > 0)
            {   
                //buffer[BUFFER_SIZE - 1] = '\0';
                // Convert text format (string) to integer
                number = atoi(buffer);
                if (number % 2 == 0)
                {
                    printf("Storing even number %u\n", number);
                    snprintf(buffer, BUFFER_SIZE, "%09d\n", number);
                    
                    // TODO - Store formated even number into the even numbers file
                    write(file_descriptor_even, buffer, BUFFER_SIZE - 1);
                }
            }

            
            // When done, you can release the lock explicitly
            lock.l_type = F_UNLCK;
            if (fcntl( prod_file_descriptor, F_SETLK, &lock) < 0) // no need to wait, command F_SETLK
                exit(-1); // error in releasing the lock
            // this lock is released
            usleep(2500000);
        }

        close(file_descriptor_even);
        
        return NULL;
    }

    int main()
    {
        pthread_t tid_1, tid_2;

        // specify lock for reading
        lock.l_type = F_RDLCK; /* F_WRLCK write lock, F_RDLCK read lock, F_UNLCK unlock */
        lock.l_whence = SEEK_SET; /* base for offset of block to be locked */
        lock.l_start = 0; /* start of block to be locked */
        lock.l_len = 0; /* its length (0 means until end of the file) */
        lock.l_pid = getpid(); /* pid of process using this lock */

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
