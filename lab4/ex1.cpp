#include <sys/wait.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
	int i, pid, status=1;

	for (i = 0; i < 10; ++i) {
		pid = fork();
	}
		 waitpid(pid,&status,WNOHANG);

	return 0;
}
