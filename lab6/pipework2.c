#include <stdio.h>
#include <unistd.h>
#include <string.h>

const int BUF_SIZE = 256;
char* message = "This is a message from the sending child process\n";

main () {
	int fd[2];
	int pid;
	char buf[BUF_SIZE];

	if (pipe(fd) < 0) {
		perror("PIPE ERROR");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		read(fd[0], buf, BUF_SIZE);
		printf("%s", buf);		
		close(fd[0]);
		return 0;
	}

	pid = fork();
	if (pid == 0) {
		write(fd[1], message, strlen(message)); 
		close(fd[1]);
		return 0;
	}
		
	close(fd[0]);
	close(fd[1]);
	wait(0);
	wait(0);

	return 0;
}
