#include <sys/wait.h>
#include <iostream>
#include <unistd.h>

using namespace std;

int main() {
	int i, pid, status;
	pid_t w;

	for (i = 0; i < 3; ++i) {
		pid = fork();

		if (pid == 0) {
			//exit(getpid() % 256);
			kill(getpid(), SIGKILL);
		}
	}

	while ((w = wait(&status)) != -1) {
		cout << "Child " << w << " status " << status << "\n";
	}

	return 0;
}
