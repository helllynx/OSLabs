# Лабораторная работа 6

## Задание 1

**1.2**

pipework.c
```
./a.out
drwxr-xr-x 2 zybc zybc 4096 май  6 10:31 lab3
drwxr-xr-x 2 zybc zybc 4096 май  8 10:59 extern_files
drwxr-xr-x 2 zybc zybc 4096 май  8 11:01 lab4
drwxr-xr-x 2 zybc zybc 4096 май  8 12:13 lab5
drwxr-xr-x 2 zybc zybc 4096 май  8 12:59 lab6
-rw-r--r-- 1 zybc zybc   33 май  8 09:27 README.md
total 24
```

ls -l ~/MIET/OS/env/
```
total 24
drwxr-xr-x 2 zybc zybc 4096 май  8 10:59 extern_files
drwxr-xr-x 2 zybc zybc 4096 май  6 10:31 lab3
drwxr-xr-x 2 zybc zybc 4096 май  8 11:01 lab4
drwxr-xr-x 2 zybc zybc 4096 май  8 12:13 lab5
drwxr-xr-x 2 zybc zybc 4096 май  8 12:59 lab6
-rw-r--r-- 1 zybc zybc   33 май  8 09:27 README.md
```



ls -l ~/MIET/OS/env/ | sort
```
drwxr-xr-x 2 zybc zybc 4096 май  6 10:31 lab3
drwxr-xr-x 2 zybc zybc 4096 май  8 10:59 extern_files
drwxr-xr-x 2 zybc zybc 4096 май  8 11:01 lab4
drwxr-xr-x 2 zybc zybc 4096 май  8 12:13 lab5
drwxr-xr-x 2 zybc zybc 4096 май  8 12:59 lab6
-rw-r--r-- 1 zybc zybc   33 май  8 09:27 README.md
total 24
```

**Как мы видим, вывод программы и команды ls -l /home/osipxd | sort совпадает.**

**1.3**

```
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char* message = "This\nis\na\nmessage\nfrom\nthe\nsending\nchild\nprocess\n";

main () {
	int fd[2];
	int pid;

	if (pipe(fd) < 0) {
		perror("PIPE ERROR");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		dup2(fd[0], 0);
		close(fd[1]);
		execlp("sort", "sort", (char*) 0);
	}

	pid = fork();
	if (pid == 0) {
		write(fd[1], message, strlen(message)); 
		close(fd[1]);
		return 1;
	}
		
	close(fd[0]);
	close(fd[1]);
	wait(0);
	wait(0);

	return 0;
}
```

```
./a.out 
a
child
from
is
message
process
sending
the
This
```

**1.3**

```
#include <stdio.h>
#include <unistd.h>
#include <string.h>

char* message = "This\nis\na\nmessage\nfrom\nthe\nsending\nchild\nprocess\n";

main () {
	int fd[2];
	int pid;

	if (pipe(fd) < 0) {
		perror("PIPE ERROR");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		dup2(fd[0], 0);
		close(fd[1]);
		execlp("sort", "sort", (char*) 0);
	}

	pid = fork();
	if (pid == 0) {
		write(fd[1], message, strlen(message)); 
		close(fd[1]);
		return 1;
	}
		
	close(fd[0]);
	close(fd[1]);
	wait(0);
	wait(0);

	return 0;
}
```
```
./a.out 1
This is a message from the sending child process
```








