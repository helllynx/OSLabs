#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {
	kill(getppid(), SIGKILL);
	return 0;
}
