/***********************
* Producer - Consumer problem using a shared file
*
* This is the **consumer** side
* -> name of shared file passed as 1st argument
*
* Launch several consumers and see how the lock allows simultaneous read access
* Launch together with producer and see the lock blocking simultaneous write access
*
*  based on the example, here:
*   https://opensource.com/article/19/4/interprocess-communication-linux-storage
*
************************/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

// error exit
void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1); /* EXIT_FAILURE */
}

int main(int argc, char *argv[]) {
  struct flock lock;

  lock.l_type = F_RDLCK;    /* read lock */
  lock.l_whence = SEEK_SET; /* base for seek offsets */
  lock.l_start = 0;         /* 1st byte in file */
  lock.l_len = 0;           /* 0 here means 'until EOF' */
  lock.l_pid = getpid();    /* process id */

  int fd; /* file descriptor to identify a file within a process */
  if ((fd = open(argv[1], O_RDONLY)) < 0)  /* -1 signals an error */
    report_and_exit("open to read failed...");

  printf("Process %d accessing the file...\n",lock.l_pid);

  lock.l_type = F_RDLCK;    /* read lock */
  if (fcntl(fd, F_SETLKW, &lock) < 0)
    report_and_exit("can't get a read-only lock...");

  /* Read the bytes (they happen to be ASCII codes) one at a time. */
  int c; /* buffer for read bytes */
  while (read(fd, &c, 1) > 0)    /* read one byte, 0 return signals EOF */
    write(STDOUT_FILENO, &c, 1); /* write one byte to the standard output */
    c = '\n';   // add a new line for better visibility
    write(STDOUT_FILENO, &c, 1); /* write one byte to the standard output */

  // keep the lock for a little longer to make effect more visible
  sleep(5);
  
  /* Release the lock explicitly. */
  lock.l_type = F_UNLCK;
  if (fcntl(fd, F_SETLK, &lock) < 0)
    report_and_exit("explicit unlocking failed...");

  close(fd);
  return 0;
}