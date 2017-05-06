#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

using namespace std;

sig_atomic_t exstatus;

void chld_hand(int number);

int main() {
	struct sigaction sigact;
	
	memset(&sigact, 0, sizeof(sigact));
	sigact.sa_handler = chld_hand;
	sigaction(SIGCHLD, &sigact, NULL);
	
	int child_pid = fork();
	if (child_pid == 0) {
		return 2;	
	}

	int counter = 0;
	while(true) {
		counter++;
		if (exstatus) {
			cout << "Операций пыполнено: " << counter 
			     << "\nКод завершения: " << exstatus << "\n";
			break;		
		}	
	}

	return 0;
}

void chld_hand(int snumber) {
	int status;
	wait(&status);
	exstatus = status;
}
