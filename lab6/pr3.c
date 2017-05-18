#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FIFONAME "myfifo"

const int BUF_SIZE = 1024;

int fd;

int sendMSG(char* message) {
	char output[BUF_SIZE];
	sprintf(output, "%d %d %s", (int) strlen(message) + 1, getTime(), message);
	write(fd, output, BUF_SIZE);

	return 0;
}

int getTime() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	return (int) currentTime.tv_usec;
}


int main(int argc, char* argv[]) {
	int i, n, pid;
	char buf[BUF_SIZE];

	unlink(FIFONAME);

	if (mkfifo(FIFONAME, 0666) < 0) {
		perror("mkfifo");
		return 1;
	}

	if (chmod(FIFONAME, 0666) < 0) {
		perror("chmod"); 
		return 1;
	}

	if ((fd = open(FIFONAME, O_RDWR)) < 0) {
		perror("open");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		return 	sendMSG("MSG from 1-st PROCESS!");
	}

	pid = fork();
	if (pid == 0) {
		return 	sendMSG("MSG from 2-d PROCESS!");
	}	

	
	pid = fork();
	for (i = 0; i < 2; i++) {
		read(fd, buf, BUF_SIZE);
		
		char* firstSpace = strchr(buf, ' ') + 1;
		char subString[6]; 
		strncpy(subString, firstSpace, 6);
		int spendedTime = getTime() - atoi(subString);
		
		puts("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		printf("MSG Text: %s\n", buf);	
		printf("Time: %d us\n", spendedTime);	
	}

	unlink(FIFONAME);
	close(fd);
	return 0;
}

