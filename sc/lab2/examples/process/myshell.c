//////////// 
// program that shows how to launch processes 
// to run different programs
// uses "execve()" system call
// the program builds a very simple shell that
// asks for commands and executes them using execve()
// ATTENTION: commands must be local or written with full path
//            examples: "ex_fork 4", "/bin/ps -a", "/bin/ls -l"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
	char cmd[20], cmd_line[100];  // space for the command and command line
	char cmd_arg1[20];  // space for the arguments in the command line
	char cmd_arg2[20];  // at most 3 arguments
	char cmd_arg3[20];
	int n_cmd_args=0, child_status;

	while (1) {
		char *new_argv[] = {NULL, NULL, NULL, NULL, NULL};  //clear the arguments

		// get a comand line and parse it in separate arguments
		printf(">> ");   // prompt of this shell
		fgets(cmd_line, 100, stdin);
		n_cmd_args = sscanf(cmd_line,"%s %s %s %s", cmd, cmd_arg1, cmd_arg2, cmd_arg3);
		if ( n_cmd_args <= 0 ) continue;
		//printf("n_cmd_args = %d \n", n_cmd_args);

		// pass the input arguments to the arguments structure
		switch (n_cmd_args){
			case 4:
				new_argv[3]=cmd_arg3;
				//printf("%s ", new_argv[3]);
			case 3:
				new_argv[2]=cmd_arg2;
				//printf("%s ", new_argv[2]);
			case 2:
				new_argv[1]=cmd_arg1;
				//printf("%s ", new_argv[1]);
			case 1:
				new_argv[0]=cmd;
				//printf("%s ", new_argv[0]);
		}
	
		// clone the process to invoke the new command
		int retval = fork();
		if (retval == 0) {
			// This is the child process
			// Setup the child’s process environment
			// e.g., where is standard i/o, how to handle signals?
			execve(cmd, new_argv ,NULL);
			// exec does not return if it succeeds
			printf("ERROR: Could not execute %s\n", cmd);
		} else { // This is the parent process
			// Wait for child to finish
			wait(&child_status);
			printf("exit status: %d\n", child_status);
			// if the above line is commented 
			// --> do nothing and go back to ask for a new command line
		}
	}
}
	