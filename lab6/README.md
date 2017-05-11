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

**1.4**


```
#include <stdio.h>
#include <unistd.h>
#include <string.h>

const int BUF_SIZE = 256;
char* message = "This is a message from the sending child process\n";

main () {
	int fd[2];
	int pid;
	char buf[BUF_SIZE];

	if (pipe(fd) < 0) {
		perror("PIPE ERROR");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		read(fd[0], buf, BUF_SIZE);
		printf("%s", buf);		
		close(fd[0]);
		return 0;
	}

	pid = fork();
	if (pid == 0) {
		write(fd[1], message, strlen(message)); 
		close(fd[1]);
		return 0;
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
This is a message from the sending child process
```


## Задание 2


fifo

```
cat > fifo1 
Hello!
WORLD!
OMG THE TEXT DUPLICATES!!!!
```

```
cat < fifo1 
Hello!
WORLD!
OMG THE TEXT DUPLICATES!!!!
```

**2.1**

client2.c
```
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int BUF_SIZE = 1024;

int main(int argc, char * argv[]) {
	int n, fd;
	char buf[BUF_SIZE];

	if (argc < 2) {
		puts("Please use ./client2 <pipename>");
		return 1;	
	}

	/*
	 * Open the pipe for writing.  It was
	 * created by the server already.
	 */
	if ((fd = open(argv[1], O_WRONLY)) < 0) {
		perror("open");
		return 1;
	}

	printf("CLIENT STARTS…\n");

	/*
	 * Read from standard input, and copy the
	 * data to the pipe.
	 * If data is input from the keyboard 
	 * then terminate input by double CTRL/D
	 */
	while ((n = read(0, buf, BUF_SIZE)) > 0) {
		write(fd, buf, n);
	}

	printf("CLIENT TERMINATED…\n");
	close(fd);
	return 0;
}
```

server2.c
```
/* Named pipe: a server program */
/* Usage: progname pipename & */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

const int BUF_SIZE = 1024;

int main(int argc, char * argv[]) {
	int n, fd;
	char buf[BUF_SIZE];
	
	if (argc < 2) {
		puts("Please use ./server2 <pipename>");
		return 1;	
	}

	/*
	 * Remove any previous FIFO.
	 */
	unlink(argv[1]);
	printf("SERVER STARTS…\n");

	/*
	 * Create the FIFO.
	 */
	if (mkfifo(argv[1], 0666) < 0) {
		perror("mkfifo");
		return 1;	
	}

	/* Make sure that the mode of the pipe is really 0666 */
	if (chmod(argv[1], 0666) < 0) {
		perror("chmod"); 
		return 1;	
	}

	/*
	 * Open the FIFO for reading.
	 * The server will block here until the client starts...
	 */
	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open");
		return 1;	
	}

	/*
	 * Read from the FIFO until end-of-file and
	 * print what we get on the standard output.
	 */
	while ((n = read(fd, buf, BUF_SIZE)) > 0) {
		write(1, buf, n);
	}
	
	printf("SERVER TERMINATED…\n");
	close(fd);
	return 0;	
}
```

```
SERVER STARTS…
OMG
IT WORKS
DONE!
SERVER TERMINATED…
```

```
./client pipe
CLIENT STARTS…
OMG
IT WORKS
DONE!
^C
```

## Задание 3

Два процесса в именованный кидают по сообщению, в начале сообщения длина и время отправки, третий процесс распечатывает их.

```
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define FIFONAME "myfifo"

const int BUF_SIZE = 1024;

int fd;

int sendMSG(char* message) {
	char output[BUF_SIZE];
	sprintf(output, "%d %d %s", (int) strlen(message) + 1, getTime(), message);
	write(fd, output, BUF_SIZE);

	return 0;
}

int getTime() {
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);

	return (int) currentTime.tv_usec;
}

int main(int argc, char* argv[]) {
	int i, n, pid;
	char buf[BUF_SIZE];

	unlink(FIFONAME);

	if (mkfifo(FIFONAME, 0666) < 0) {
		perror("mkfifo");
		return 1;
	}

	if (chmod(FIFONAME, 0666) < 0) {
		perror("chmod"); 
		return 1;
	}

	if ((fd = open(FIFONAME, O_RDWR)) < 0) {
		perror("open");
		return 1;
	}

	pid = fork();
	if (pid == 0) {
		return 	sendMSG("MSG from 1-st PROCESS!");
	}

	pid = fork();
	if (pid == 0) {
		return 	sendMSG("MSG from 2-d PROCESS!");
	}	

	pid = fork();
	for (i = 0; i < 2; i++) {
		read(fd, buf, BUF_SIZE);
		
		char* firstSpace = strchr(buf, ' ') + 1;
		char subString[6]; 
		strncpy(subString, firstSpace, 6);
		int spendedTime = getTime() - atoi(subString);
		
		puts("-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");
		printf("MSG Text: %s\n", buf);	
		printf("Time: %d us\n", spendedTime);	
	}

	unlink(FIFONAME);
	close(fd);
	return 0;
}
```


```
./a.out 
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
MSG Text: 23 111916 MSG from 1-st PROCESS!
Time: 69 us
-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
MSG Text: 22 112144 MSG from 2-d PROCESS!
Time: 81 us
```