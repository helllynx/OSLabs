#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

using namespace std;

const int BUF_SIZE = 128;

void alrm_hand(int number);

int main() {
	struct sigaction sigact;
	
	memset(&sigact, 0, sizeof(sigact));
	sigact.sa_handler = alrm_hand;
	sigact.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &sigact, NULL);
	
	alarm(5);
	char buffer[BUF_SIZE];
	if (read(0, buffer, BUF_SIZE) == -1) {
		if (errno == EINTR) {
			cout << "Время истекло\n";		
		}
	} else {
		cout << "Привет, " << buffer;
	}

	return 0;
}

void alrm_hand(int snumber) {
	// empty
}
