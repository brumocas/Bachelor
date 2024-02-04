////////////
// program that reads an existing file
// and writes its content to the terminal
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* File display program. Minimal error checking */
#define BUF_SIZE 256

int main(int argc, char *argv[]) {
	int in_fd, rd_cnt, wr_cnt, offset;
	char buf[BUF_SIZE], *aux;

	if (argc != 2) /* incorrect number of args */
		exit(1);

	in_fd = open(argv[1], O_RDONLY); /* open source file */
	if (in_fd < 0 )
		exit(2); /* error in open */

//	lseek( in_fd, 0, SEEK_CUR);
	
	while (1) { /* loop until done, or an error */
		rd_cnt = read(in_fd, buf, BUF_SIZE); /* read from source */
		if (rd_cnt <= 0)
			break; /* end of file, or error */
		offset=0;
		aux=buf;
		while(offset<rd_cnt){  //note that buf has several strings inside
			offset += strlen(aux);
			offset++;   // include the null termination
			printf("%s", aux); /* write one string */
			aux = buf+offset;  // adjust pointer aux to start of next string
		}
		
		// Print new line to have better view of the file
		printf("\n");

//		wr_cnt = write(1, buf, rd_cnt); /* write block read */
//		if (wr_cnt < 0)
//			exit(4); /* error writing */
	}
	close( in_fd); /* close files */
	if( rd_cnt == 0 ) /* no error on last read */
		exit(0);
	else /* error on last read */
		exit(5);
}