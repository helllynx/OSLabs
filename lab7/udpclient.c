/*           udpclient.c		*/
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#define SERV_PORT 20001  			
#define BUFSIZE 1024
#define SADDR struct sockaddr
#define SLEN sizeof(struct sockaddr_in)

int main(int argc, char** argv) {
	int sockfd, n;
	char sendline[BUFSIZE], recvline[BUFSIZE + 1];
	struct sockaddr_in servaddr; 			
	struct sockaddr_in cliaddr; 			

	if (argc != 2) {
		printf("Usage: client <IPaddress of server>\n"); 
		return 1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0) {
		perror("inet_pton problem"); 
		return 1;
	}
	
	if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket problem"); 
		return 1;
	}

	write(1, "Enter string\n", 13);		
	while ((n = read(0, sendline, BUFSIZE)) > 0) { 					 	
		if (sendto(sockfd, sendline, n, 0, (SADDR*) &servaddr, SLEN) == -1) {
			perror("sendto problem"); 
			return 1;
		}

		if (recvfrom(sockfd, recvline, BUFSIZE, 0, NULL, NULL) == -1) {
			perror("recvfrom problem");
			return 1;
		}

		printf("REPLY FROM SERVER = %s\n", recvline);
	}
	
	close(sockfd);	
	return 0;				
}
