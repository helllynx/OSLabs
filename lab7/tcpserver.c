/*           tcpserver.c		*/
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>

#define SERV_PORT 30012				
#define BUFSIZE 100
#define SADDR struct sockaddr 
#define SIZE sizeof(struct sockaddr_in)

int main() {
	int lfd, cfd;
	int nread, clilen;
	char buf[BUFSIZE];
	struct sockaddr_in servaddr;			
	struct sockaddr_in cliaddr; 			

	if ((lfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		return 1;
	}

	memset(&servaddr, 0, SIZE); 	
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 	
	servaddr.sin_port = htons(SERV_PORT); 
			
	if (bind(lfd, (SADDR *) &servaddr, SIZE) < 0) {
		perror("bind"); 
		return 1;
	}
			
	if (listen(lfd, 5) < 0) {
		perror("listen"); 
		return 1;
	}

	for(;;){	
		clilen = SIZE;

		if ((cfd = accept(lfd, (SADDR*) &cliaddr, &clilen)) < 0) {
			perror("accept");
			return 1;
		}
		
		printf("connection established\n");
		while ((nread = read(cfd, buf, BUFSIZE)) > 0) {
			write(1, &buf, nread);
		}
		
		if (nread == -1) {
			perror("read");
			return 1;
		}
		
		close(cfd);
	}	
	
	return 0;
}

