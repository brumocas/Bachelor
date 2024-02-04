////////////
// program that opens/creates a file
// and appends strings read from the terminal
//
// play with the flags 
//   - remove O_CREAT and see when file does not exist
//   - replace O_APPEND with O_TRUNC


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>

#define BUF_SIZE 256

int main(int argc, char *argv[]) {
	int fd, s, wr_cnt;
	char line[BUF_SIZE];

	assert(argc==2);

	fd = open(argv[1], O_WRONLY|/*O_TRUNC*/O_APPEND|O_CREAT); /* open/create file for append */
	assert(fd>=0);

	while (1) { /* loop until line starts with 'X' */
		scanf("%s", line);
		if(line[0]=='X') break;
		s=strlen(line);
		line[s]=13;  // carriage return
		line[s+1]=10;  // new line
		line[s+2]=0;  // null terminated 
		wr_cnt = write(fd, line,s+2); /* write line */ 
		assert(wr_cnt >= 0);
	}
	close(fd); /* close files */
}