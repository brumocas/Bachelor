#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// Error exit
void report_and_exit(const char* msg) {
  perror(msg);
  exit(-1);
}

// Producer process/thread. Receives the file descriptor and string to be written.
int producer(int fd, char *str) {
  
  // Write string to the device and print a message.
  write(fd, str, strlen(str));
  printf("Producer wrote data '%s' to file...\n", str);
  printf("Returning from producer\n");
  
  return 0;
}

// Consumer process/thread. Receives the file descriptor. It should read from a file.
int consumer(int fd) {
  // Give time to start the producer.
  sleep(1);

  // Read the characters stored on the device, one at a time.
  char buffer[256];
  ssize_t bytesRead;
  while ((bytesRead = read(fd, buffer, sizeof(buffer))) > 0) {
    printf("Consumer read data: %.*s\n", (int)bytesRead, buffer);
  }

  printf("Returning from consumer\n");
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3)
    report_and_exit("Not enough arguments...");

  int file_descriptor = open(argv[1], O_RDWR);
  if (file_descriptor < 0)
    report_and_exit("Both open failed...");

  // Launch the producer/consumer processes.
  pid_t pid = fork();

  if (pid < 0) {
    report_and_exit("Fork failed...");
  } else if (pid == 0) {

    // Child process - producer
    printf("Process (%d): executing the consumer\n",getpid());
    producer(open("/dev/mypipe", O_WRONLY), argv[2]);

    close(file_descriptor); // Close unused file descriptor
    exit(0);

  } 
  else {
    // Parent process - consumer
    printf("Process (%d): executing the producer\n",getpid());
    consumer(file_descriptor);
  }
  // wait for child process
  wait(NULL);
  printf("parent: terminating\n");


  close(file_descriptor);
  return 0;
}
