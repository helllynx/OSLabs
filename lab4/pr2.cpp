#include <sys/wait.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {
	int pid = fork();

	if (pid == 0) {
		return 0;
	}

	pid = wait(NULL);
	cout << "\nParent: " << getpid() << " Child: " << pid << "\n";

	sleep(30);
	
	return 0;
}
