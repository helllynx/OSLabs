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



## Контрольные вопросы

**1. Каково назначение системных вызовов wait и waitpid?**

Функция wait приостанавливает выполнение текущего процесса до тех пор, пока дочерний процесс не завершится, или до появления сигнала, который либо завершает текущий процесс, либо требует вызвать функцию-обработчик. Если дочерний процесс к моменту вызова функции уже завершился (так называемый "зомби" ("zombie")), то функция немедленно возвращается. Системные ресурсы, связанные с дочерним процессом, освобождаются. Функция waitpid приостанавливает выполнение текущего процесса до тех пор, пока дочерний процесс, указанный в параметре pid, не завершит выполнение, или пока не появится сигнал, который либо завершает текущий процесс либо требует вызвать функцию-обработчик. Если указанный дочерний процесс к моменту вызова функции уже завершился (так называемый "зомби"), то функция немедленно возвращается. Системные ресурсы, связанные с дочерним процессом, освобождаются. Параметр pid может принимать несколько значений: 


**2. Что такое состояние "зомби" процесса?**

Дочерний процесс в Unix-системе, завершивший своё выполнение, но ещё присутствующий в списке процессов операционной системы, чтобы дать родительскому процессу считать код завершения.

Когда процесс завершается, информация о нем сохраняется в системных
таблицах для того, чтобы родительский процесс мог получить информацию о нем, в частности,
его код возврата при помощи функции wait. Если же родительский процесс не вызывает wait,
то дочерний процесс продолжает существовать в так называемом состоянии "зомби". Наличие
таких процессов нежелательно, т.к. они занимают место в системной таблице процессов. Если
родительский процесс завершится, не вызвав wait, то процесс-потомок удаляется из памяти
процессом init.

**3. Как в родительском процессе можно узнать о завершении дочернего процесса, не прерывая основной работы?**

Получить асинхронное уведомление о завершении дочернего процесса можно при помощи сигнала SIGCHLD, который ОС посылает родительскому процессу при завершении его потомка.

**4. Как в программе задать реакцию на сигнал определенного типа?**

Функция обработчик, есть в лабе.

**5. Все ли функции можно использовать в обработчике сигналов?**

Из обработчика сигнала нельзя выполнять общий c другими
частями процесса нереентерабельный участок кода, если он не защищается
всякий раз путем блокировки соответствующих сигналов.

**6. Как послать сигнал определенному процессу? группе процессов? всем процессам?**

int kill(pid_t pid, int sig);

ОПИСАНИЕ
Системный вызов kill используется для того, чтобы послать
любой сигнал любому процессу или группе процессов.

Если pid больше 0, то сигнал sig посылается процессу pid.

Если pid равен 0, то сигнал sig посылается всем процессам
текущей группы.

Если pid равен -1, то сигнал sig посылается всем
процессам, кроме процесса 1 (init). Также смотрите
замечания ниже.

Если pid меньше -1, то сигнал sig посылается всем
процессам группы -pid.

Если sig равен 0, то сигнал не посылается, но выполняется
проверка на возникновение ошибок в процессе.

**7. Может ли обработчик сигналов быть прерван сигналом другого типа? аналогичного типа?**


 Выполнение обработчика сигнала может быть
прервано очередным доставленным сигналом.  Чтобы избежать прерывания,
необходимо заблокировать все или некоторые сигналы на время работы
чувствительного участка в обработчике.  Проще всего это делать,
указав маску sa_mask в параметре sigaction(2); эта маска будет
атомарно установлена на входе в обработчик, а на выходе из него
будет восстановлено предыдущее значение маски сигналов.

Если для текущего сигнала не установлен флаг SA_RESTART, то
некоторые системные вызовы будут прерваны с ошибкой EINTR.  Если
же флаг SA_RESTART установлен, то выполнение этих вызовов будет
продолжено.



**8. Как в обработчике сигналов блокировать поступление сигналов определенных типов?**

 Поскольку игнорирование сигнала устанавливается функцией sigaction(), можно было бы ожидать, что и блокировка устанавливается этой же функцией, но это не так. Так как зачастую программисту приходится блокировать несколько сигналов сразу, для блокировки существует специальная функция sigprocmask(2), которая оперирует наборами сигналов (signal sets). Разделение интерфейса между несколькими функциями вызвано еще и требованиями многопоточности. Параметры, устанавливаемые sigaction(), действительны для всей программы в целом, тогда как блокировку сигналов потоки осуществляют независимо друг от друга. Наборы сигналов хранятся в переменных специального типа - sigset_t, а операции над ними осуществляются с помощью специальных функций. Функция sigemptyset() инициализирует набор сигналов пустыми значениями, а функция sigfillset() устанавливает все возможные значения в наборе. Используемая нами функция sigaddset() добавляет значение сигнала в набор, а функция sigdelset() удаляет сигнал из набора. После того как набор сигналов сформирован, мы передаем его функции sigprocmask(), которая выполняет блокирование и разблокирование сигналов. 

**9. Что происходит, если сигнал поступает во время блокировки процесса на системном вызове, например, когда процесс ожидает ввода с клавиатуры?**


**10. Каково назначение директивы #define в программе procmemory.c?**

#define SHW_ADR(ID,I) (printf("ID %s \t is at virtual address: %8p\n", ID, &I)) 

ну как бы, тут нужны комментарии?

**11. Каков смысл переменных etext, edata и end в программе procmenory.c? Почему эти переменные объявлены с атрибутом extern?**

что бы они были глобальными (ваш кэп)

**12. В родительском процессе некоторой переменной было присвоено значение до создания дочернего процесса. Будет ли эта переменная доступна в дочернем процессе? Каково будет ее значение в начальный момент времени? Если родительский процесс будет изменять значение переменной, то будет ли это изменение доступно дочернему процессу? А наоборот? **
