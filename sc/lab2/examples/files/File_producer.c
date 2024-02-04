/***********************
* Producer - Consumer problem using a shared file
*
* This is the **producer** side
* -> name of shared file passed as 1st argument
* -> data to write as a string in the 2nd argument
*
* Launch several producers and see how the lock blocks simultaneous write access
*
*  based on the example, here:
*   https://opensource.com/article/19/4/interprocess-communication-linux-storage
*
************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>


// error exit
void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

int main(int argc, char *argv[]) {
  struct flock lock;        // get space for one lock (name=lock, type=struct flock

  lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
  lock.l_whence = SEEK_SET; /* base for seek offsets */
  lock.l_start = 0;         /* 1st byte in file */
  lock.l_len = 0;           /* 0 here means 'until EOF' */
  lock.l_pid = getpid();    /* process id */

  if( argc < 3 )
	  report_and_exit("not enough arguments...");

  int fd; /* file descriptor to identify a file within a process */
  if ((fd = open(argv[1], O_RDWR | O_CREAT | /*O_TRUNC*/O_APPEND, 0644)) < 0)  /* -1 signals an error */
    report_and_exit("open failed...");

  lock.l_type = F_WRLCK;    /* read/write (exclusive versus shared) lock */
  if (fcntl(fd, F_SETLKW, &lock) < 0)   /* F_SETLK doesn't block, F_SETLKW does */
    report_and_exit("fcntl failed to get lock...");
  else {
    write(fd, argv[2], strlen(argv[2])); /* populate data file */
    write(fd, "\n", 1);
    fprintf(stderr, "Process %d has written to data file...\n", lock.l_pid); 
  }

  // keep the file locked for 5s to make it visible
  // normally you wouldn't do this!!
  sleep(10);
  
  /* Now release the lock explicitly. */
  lock.l_type = F_UNLCK;
  if (fcntl(fd, F_SETLK, &lock) < 0)
    report_and_exit("explicit unlocking failed...");

  close(fd); /* close the file: would unlock if needed */
  return 0;  /* terminating the process would unlock as well */
}