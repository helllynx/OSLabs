/* Named pipe: a server program */
/* Usage: progname pipename & */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FIFONAME "myfifo"

const int BUF_SIZE = 1024;

int main(int argc, char * argv[]) {
	int n, fd;
	char buf[BUF_SIZE];
	printf("SERVER STARTS…\n");
	
	/*
	 * Remove any previous FIFO.
	 */
	unlink(FIFONAME);

	/*
	 * Create the FIFO.
	 */
	if (mkfifo(FIFONAME, 0666) < 0) {
		perror("mkfifo");
		exit(1);
	}

	/* Make sure that the mode of the pipe is really 0666 */
	if (chmod(FIFONAME, 0666) < 0) {
		perror("chmod"); 
		exit(1);
	}

	/*
	 * Open the FIFO for reading.
	 * The server will block here until the client starts...
	 */
	if ((fd = open(FIFONAME, O_RDONLY)) < 0) {
		perror("open");
		exit(1);
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
	exit(0);
}

