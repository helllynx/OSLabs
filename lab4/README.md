# Лабораторная работа 4


## Задание 1

**1.1**

![alt text](https://github.com/yenqwerty/OSLabs/blob/master/extern_files/diagram.jpg)

**1.2**

```
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
			exit(getpid() % 256);
			// kill(getpid(), SIGKILL);
		}
	}

	while ((w = wait(&status)) != -1) {
		cout << "Child " << w << " status " << status << "\n";
	}

	return 0;
}
```

```
./a.out 
Child 5714 status 20992
Child 5715 status 21248
Child 5716 status 21504
```


```
// kill(getpid(), SIGKILL);
```

```
./a.out 
Child 5831 status 9
Child 5832 status 9
Child 5833 status 9
```
В качестве кода возврата в первом случае мы отправляли PID процесса, а во втором случае отправляем сигнал SIGKILL, который имеет значение 9.


**1.3**

```
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
```

Результат работы и вызов команды ps -mAl (я заменил на ps -ml, чтобы не отображалось излишне много процессов):

```
ps -ml
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 -  1000  9570  5690  0   -   - -  4323 -      pts/1    00:00:00 bash
0 S  1000     -     -  0  80   0 -     - wait   -        00:00:00 -
0 -  1000  9575  9570  0   -   - -  8453 -      pts/1    00:00:00 ps
0 R  1000     -     -  0  80   0 -     - -      -        00:00:00 -
```

## Задание 2

**2.1**

```
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

using namespace std;

int main() {
	kill(getppid(), SIGKILL);
	return 0;
}
```
Если мы используем SIGTERM, то bash просто игнорирует этот сигнал, но если используем SIGKILL, то bash немедленно завершает свою работу.


Номера сигналов:
SIGTERM 	: 15
SIGKILL		: 9



**2.2**

```
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
			cout << "Operations done: " << counter 
			     << "\nExit code: " << exstatus << "\n";
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

```

```
./a.out 
Operations done: 80533
Exit code: 512
```

**2.3**

```
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
```

```
./a.out 
Yanis
Hi, Yanis

./a.out 
Time ended!
```

EINTR возникает из-за того, что поступает сигнал прерывания инициируемый alarm’om
SA_RESTART повлияет на работу программы работу – сигнал прерывания будет проигнорирован.


## Задание 3.


**3.1**

```
./a.out 

Address etext: 0x40096d
Address edata: 0x601070
Address end  : 0x6010a0
ID main 	 is at virtual address: 0x400716
ID showit 	 is at virtual address: 0x40083f
ID cptr 	 is at virtual address: 0x601068
ID buffer1 	 is at virtual address: 0x601080
ID i	 	 is at virtual address: 0x7ffff044b6fc
A demonstration
ID buffer2 	 is at virtual address: 0x7ffff044b6d8
Alocated memory at 0xae0030
This message is output by the function showit()
```


![alt text](https://github.com/yenqwerty/OSLabs/blob/master/extern_files/diagram_procmem.jpg)


**2.2**
после добавления fork();


```
./a.out 

Address etext: 0x4009bd
Address edata: 0x601078
Address end  : 0x6010b0
ID main 	 is at virtual address: 0x400766
ID showit 	 is at virtual address: 0x400894
ID cptr 	 is at virtual address: 0x601070
ID buffer1 	 is at virtual address: 0x601090
ID i	 	 is at virtual address: 0x7ffcc3ddcacc
A demonstration
ID buffer2 	 is at virtual address: 0x7ffcc3ddcaa8
Alocated memory at 0x1478030
This message is output by the function showit()

Address etext: 0x4009bd
Address edata: 0x601078
Address end  : 0x6010b0
ID main 	 is at virtual address: 0x400766
ID showit 	 is at virtual address: 0x400894
ID cptr 	 is at virtual address: 0x601070
ID buffer1 	 is at virtual address: 0x601090
ID i	 	 is at virtual address: 0x7ffcc3ddcacc
A demonstration
ID buffer2 	 is at virtual address: 0x7ffcc3ddcaa8
Alocated memory at 0x1478030
This message is output by the function showit()
```