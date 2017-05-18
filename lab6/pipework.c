#include <stdio.h>
#include <unistd.h>

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
		dup2(fd[1], 1);
		close(fd[0]);
		execlp("ls", "ls", "-l", "/home/zybc/MIET/OS/env/", (char*) 0);
	}
		
	close(fd[0]);
	close(fd[1]);
	wait(0);
	wait(0);

	return 0;
}
