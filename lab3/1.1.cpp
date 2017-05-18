#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <iostream>

int main(){
	fork();
	printf("A\n");
	fork();
	printf("B\n");
	execlp("pstree","pstree","489",NULL);
	return 0;
}
