/* Named pipe: a server program */
/* Usage: progname pipename & */

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
		puts("Please use ./server2 <pipename>");
		return 1;	
	}

	/*
	 * Remove any previous FIFO.
	 */
	unlink(argv[1]);
	printf("SERVER STARTS…\n");

	/*
	 * Create the FIFO.
	 */
	if (mkfifo(argv[1], 0666) < 0) {
		perror("mkfifo");
		return 1;	
	}

	/* Make sure that the mode of the pipe is really 0666 */
	if (chmod(argv[1], 0666) < 0) {
		perror("chmod"); 
		return 1;	
	}

	/*
	 * Open the FIFO for reading.
	 * The server will block here until the client starts...
	 */
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		return 1;	
	}

	/*
	 * Read from the FIFO until end-of-file and
	 * print what we get on the standard output.
	 */
	while ((n = read(fd, buf, BUF_SIZE)) > 0) {
		write(1, buf, n);
	}
	
	printf("SERVER TERMINATED…\n");
	close(fd);
	return 0;	
}

