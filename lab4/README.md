# Лабораторная работа 4


## Задание 1

**1.1**

![alt text](http://url/to/img.png)

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

**1.3**

```
// kill(getpid(), SIGKILL);
```

```
./a.out 
Child 5831 status 9
Child 5832 status 9
Child 5833 status 9
```


