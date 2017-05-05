#include <stdio.h>
#include <unistd.h>

int main(){
	fork();
	printf("A\n");
	fork();
	printf("B\n");

	sleep(100);
	return 0;
}