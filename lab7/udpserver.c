/*           udpserver.c		*/
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 20001 
#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main() {
	int sockfd, n, len;
	char mesg[BUFSIZE], ipadr[16];
	struct sockaddr_in servaddr; 	
	struct sockaddr_in cliaddr; 	

	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket problem"); 
		return 1;
	}

	memset(&servaddr, 0, SLEN);	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(SERV_PORT); 

	if (bind(sockfd, (SADDR *) &servaddr, SLEN) < 0) {
		perror("bind problem");
		return 1;
	} 		
	
	printf("SERVER starts...\n");
	for (;;) {  	
		len = SLEN;		

		if ((n = recvfrom(sockfd, mesg, BUFSIZE, 0, (SADDR*) &cliaddr, &len)) < 0) {
			perror("recvfrom"); 
			return 1;
		}
		mesg[n] = 0; 	

		printf("REQUEST %sFROM %s : %d\n", mesg, inet_ntop(AF_INET, (void*) &cliaddr.sin_addr.s_addr, ipadr,16), ntohs(cliaddr.sin_port));

		sleep(10);
		if (sendto(sockfd, mesg, n, 0, (SADDR*) &cliaddr, len) < 0) {
			perror("sendto"); 
			return 1;
		} 
	}
	
	return 0;
}

