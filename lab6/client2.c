/* Named pipe: a client program */
/* Usage: progname pipename */
/*    or: progname pipename < text-file */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int BUF_SIZE = 1024;

int main(int argc, char * argv[]) {
	int n, fd;
	char buf[BUF_SIZE];

	if (argc < 2) {
		puts("Please use ./client2 <pipename>");
		return 1;	
	}

	/*
	 * Open the pipe for writing.  It was
	 * created by the server already.
	 */
	if ((fd = open(argv[1], O_WRONLY)) < 0) {
		perror("open");
		return 1;
	}

	printf("CLIENT STARTS…\n");

	/*
	 * Read from standard input, and copy the
	 * data to the pipe.
	 * If data is input from the keyboard 
	 * then terminate input by double CTRL/D
	 */
	while ((n = read(0, buf, BUF_SIZE)) > 0) {
		write(fd, buf, n);
	}

	printf("CLIENT TERMINATED…\n");
	close(fd);
	return 0;
}

