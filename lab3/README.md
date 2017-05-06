
# Лабораторная работа 4


## Задание 1.

**1.1**

```
[zybc@writer lab3]$ gcc pr1.c
[zybc@writer lab3]$ ./a.out
A
B
A
B
B
B
```


**1.2**

```
#include <stdio.h>
#include <unistd.h>
int main()
{
    fork(); printf("A\n");
    fork(); printf("B\n");
    
    sleep(100);
    return 0;
}
```

```    
ps -e -o pid,ppid,start_time,command
 2065  1843 15:36 ./a.out
 2066  2065 15:36 ./a.out
 2067  2065 15:36 ./a.out
 2068  2066 15:36 ./a.out
 2073  1916 15:36 ps -e -o pid,ppid,start_time,command
```

```
ps -f -a
 UID        PID  PPID  C STIME TTY          TIME CMD
zybc      1758  1632  0 19:19 pts/0    00:00:00 ./a.out
zybc      1759  1758  0 19:19 pts/0    00:00:00 ./a.out
zybc      1760  1758  0 19:19 pts/0    00:00:00 ./a.out
zybc      1761  1759  0 19:19 pts/0    00:00:00 ./a.out
zybc      1765  1735  0 19:19 pts/1    00:00:00 ps -f -a
```
                   
ps -la              
PPID – PID родительского процесса
F – является ли процесс форком от другого

```
F S   UID   PID  PPID  C PRI  NI ADDR SZ WCHAN  TTY          TIME CMD
0 S  1000  1758  1632  0  80   0 -  1046 hrtime pts/0    00:00:00 a.out
1 S  1000  1759  1758  0  80   0 -  1046 hrtime pts/0    00:00:00 a.out
1 S  1000  1760  1758  0  80   0 -  1046 hrtime pts/0    00:00:00 a.out
1 S  1000  1761  1759  0  80   0 -  1046 hrtime pts/0    00:00:00 a.out
0 R  1000  1791  1735  0  80   0 -  8453 -      pts/1    00:00:00 ps
```

```
 a.out─┬─a.out───a.out
         └─a.out
```

**1.3**

```
#include <iostream>
#include <unistd.h>

using namespace std;


int main()
{

    pid_t pid = fork();

    if(pid==0){
        cout<<"Child process with PID = "<<getpid()<<endl;
        cout<<"PPID (1) ="<<getppid()<<endl;
        sleep(10);
        cout<<"PPID (2) = "<<getppid()<<endl;
    } else{
        cout<<"PPID (1)"<<getpid()<<endl;
        cout<<"Child process with PID = "<<pid<<endl;
        sleep(5);
    }

    return 0;
}
```

```
./a.out
PPID (1)7442
Child process with PID = 7443
Child process with PID = 7443
PPID (1) =7442
PPID (2) = 1494
```

```
PID   TTY   TIME        CMD
1494  ?     00:00:00    upstart

```




**1.4**

```
#include <iostream>
#include <unistd.h>

using namespace std;

int main()
{
    pid_t pid = fork();
    if(pid != 0){
        pid = fork();
        if(pid==0){
            pid = fork();
            if(pid!=0){
                fork();
            }
        }
    }

    sleep(10);
    return 0;
}
```

```
  a.out─┬─a.out
        └─a.out───2*[a.out]
```


## Задание 2.

```
#include <stdio.h>
#include <unistd.h>
int main()
{
    char* arg_list[] = {"ls", "-l", "/", NULL};
    if (fork() == 0)
        {
        execvp("ls", arg_list);
        printf("Return after exec\n");

        }
    return 0;
}
```

успешно выводит /

```
execlp("ls", "ls", "-l", "/", NULL);
```

тоже самое

если подменить программы на не существующую

```
 execlp("wtf", "ls", "-l", "/", NULL);
```
```
./a.out
Return after exec
```

```
 execlp("./fork", "ls", "-l", "/", NULL);

A
B
A
B
B
B
```

## Задание 3.

**3.1**

```
#include <stdio.h>
#include <fcntl.h>

main (int argc, char* argv[])
{
    int fdrd,fdwt;
    char c;
    char parent = 'P';
    char child ='C';
    int pid;
    unsigned long i;

    if (argc != 3){
        printf("ERROR (Wrong count parameters!)");
        return 1;

    }
    if ((fdrd = open(argv[1], O_RDONLY)) == -1){
        perror("ERROR (Cen't open file!)");
        return 1;
    }
    if ((fdwt = creat(argv[2], 0666)) == -1){
        perror("ERROR (Cen't create )");
        return 1;
    }
    printf("Parent: creating a child process\n");
    pid = fork ();
    if (pid == 0)
       {
       printf("Child process starts, id = %d\n",getpid());
       for (;;)
              {
              if (read (fdrd, &c, 1) != 1) break;
              for (i=0; i<50000;i++);   /* Long cycle */
              write(1,&child,1);
              write (fdwt, &c, 1);
              
              }
       exit (0);
       }
    else
       {
       printf("Parent starts, id= %d\n", getpid());
       for (;;)
              {
              if (read (fdrd, &c, 1) != 1) break;
              for (i=0;i<50000;i++);  /* Long cycle */
              write(1,&parent,1);
              if(write(fdwt, &c, 1)==-1){
                    perror("ERROR (Cen't write into file)");
              }

              }
       wait(5);
       }
}
```
                   
Результаты     выполнения программы каждый раз разные.     Это обусловлено тем, что между родительским     и дочерним процессом каждый раз     по-разному распределяется процессорное время и они каждый раз в разном порядке     работают с файлом. Причем запись в файл     идет после сравнительно большого     промежутка времени после чтения. Из-за     этого символы могут перепутаться или     потеряться.
