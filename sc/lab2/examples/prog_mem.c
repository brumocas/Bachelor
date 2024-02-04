////////////
// program that shows the isolated memory of processes 
// asks for an int pointer, then fill in the int position
// writes the addresses and the value on the terminal
// the program does not terminate and continues running
//
// note: *** run multiple processes with this pogram
//       (run them in the backgound '&' to continue
//		  having access to the keyboard (stdin) ) 
//
// purpose: look at the addresses written by the processes,
//			should be equal, though they hold different values!!!


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "usage: %s <value>\n", argv[1]);
		exit(1);
	}
	int *p; // memory for pointer is on "stack"
	p = malloc(sizeof(int)); // malloc’d memory is on "heap"
	assert(p != NULL);
	printf("(pid:%d) addr of p: %p\n",
		(int) getpid(), &p);
	printf("(pid:%d) addr stored in p: %p\n",
		(int) getpid(), p);
	*p = atoi(argv[1]); // assign value to addr stored in p
	printf("(pid:%d) value of p: %d\n", getpid(), *p);
	// enter cycle that does nothing, just to keep the process alive
	while (1) {
		sleep(2);
/*		(*p)++;
		printf("(pid:%d) value of p: %d\n", getpid(), *(p++));
*/	}
	return 0;
}