#include <stdio.h>
#include <unistd.h>
#include <string.h>

char* message = "This\nis\na\nmessage\nfrom\nthe\nsending\nchild\nprocess\n";

main () {
	int fd[2];
	int pid;

	if (pipe(fd) < 0) {
		perror("PIPE ERROR");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		dup2(fd[0], 0);
		close(fd[1]);
		execlp("sort", "sort", (char*) 0);
	}

	pid = fork();
	if (pid == 0) {
		write(fd[1], message, strlen(message)); 
		close(fd[1]);
		return 1;
	}
		
	close(fd[0]);
	close(fd[1]);
	wait(0);
	wait(0);

	return 0;
}
