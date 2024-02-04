////////////
// program that shows the process cloning creating
// new processes with the "fork" systems call
//
// note: *** run this process with an argument that 
//       is the number of processes that will be created
//
// purpose: run this program in the backgounr ('&') 
//		and observe the multiple processes with the ps command


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	int child_status;
	
	if (argc != 2) {
		fprintf(stderr, "usage: %s <n of children>\n", argv[0]);
		exit(1);
	}

	int n = atoi(argv[1]);
	for(int i=0; i< n; i++) {
		
		int retval = fork();
		if (retval == 0) {
			// This is the child process
			printf("child %d: pid=%d; parent pid=%d\n", i+1, getpid(), getppid());
			sleep(3+i);
			printf("child %d: pid=%d; terminating...\n", i+1, getpid());
			_exit(0);
		} else { // This is the parent process
			printf("parent: pid=%d; parent pid=%d\n", getpid(), getppid());
		}
	}
	// Wait for children to finish
	for(int i=0; i< n; i++) {
		printf("parent: child %d terminated\n", wait(&child_status));
	}	
}
	