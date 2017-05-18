/*           tcpclient.c		*/
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#define BUFSIZE 100
#define SADDR struct sockaddr 
#define SIZE sizeof(struct sockaddr_in)

int main(int argc, char *argv[]) {
	int fd;
	int nread;
	char buf[BUFSIZE];
	struct sockaddr_in servaddr;

	if (argc < 3) {
		printf("Too few arguments \n"); 
		return 1;
	}
	
	if ((fd = socket(AF_INET, SOCK_STREAM, 0))<0) {
		perror("socket creating"); 
		return 1;
	}
	
	memset(&servaddr, 0, SIZE); 		
	servaddr.sin_family = AF_INET;
	
	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		perror("bad address");
		return 1;
	}
	
	servaddr.sin_port = htons(atoi(argv[2]));
	
	if (connect(fd, (SADDR*) &servaddr, SIZE) < 0) {
		perror("connect"); 
		return 1;
	}
	
	write(1, "Input message to send\n", 22);	
	while ((nread = read(0, buf, BUFSIZE)) > 0) {
		if (write(fd, buf, nread) < 0) {
			perror("write"); 
			return 1;
		}
	}
	
	close(fd);	
	return 0;
}

