# Лабораторная работа 5

## Задание 1

Главынй поток в цикле считывает с клавы строку, создает поток и пихает в него строку. Функция потока создает файл с именем file<thread_id>.dat, записывает в него эту строку и возвращает код, если все ок, то выведет успешно, если нет, то и ежику понятно)



```
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include <iostream>
#include <fstream>

using namespace std;

void* run(void* arg);

const int BUF_SIZE = 256;
int result;

int main() {
	int* error;
	pthread_t tid;
	char buffer[BUF_SIZE];

	cin.getline(buffer, BUF_SIZE);

	pthread_create(&tid, NULL, run, (void*) buffer);
	pthread_join(tid, (void**) &error);
	
	cout << (*error ? "Failure" : "Successfully") << "\n";
}

void* run(void* arg) {
	cout << "Thread ID: " << pthread_self() << "; process ID: " << getpid() << ";\n";
	result = 0;

	stringstream fileName;
	fileName << "file" << pthread_self() << ".dat";
	ofstream file;
	file.open(fileName.str().c_str());
	
	if (file.is_open()) {
		file << (char*) arg;
		file.close();
	} else {
		result = 1;	
	}
	
	return (void*) &result;
}
```




## Задание 2
mutex

```
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>

void do_one_thing(int* pnum_times);
void do_another_thing(int* pnum_times);
void do_wrap_up(int counter);

int common = 0; /* A shared variable for two threads */
pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

/* extern int */
int main() {
	pthread_t thread1, thread2;

	if (pthread_create(&thread1, NULL, (void*) do_one_thing, (void*) &common)) {
		perror("pthread_create");
		return 1; 
	}
	
	if (pthread_create(&thread2, NULL, (void*) do_another_thing, (void*) &common)) {
		perror("pthread_create");
		return 1; 
	}
  
	if (pthread_join(thread1, NULL)) {
		perror("pthread_join");
		return 1; 
	}
	
	if (pthread_join(thread2, NULL)) {
		perror("pthread_join");
		return 1; 
	}
		
	do_wrap_up(common);

	return 0; 
}

void do_one_thing(int* pnum_times) {
	int i, k, work;
  
	for (i = 0; i < 50; i++){
		// pthread_mutex_lock(&mut);
		printf("doing one thing\n");
		work = *pnum_times;
		printf("counter = %d\n", work); 
		work++; /* increment, but not write */
		for (k = 0; k < 500000; k++); /* long cycle */
		*pnum_times = work; /* write back */
		// pthread_mutex_unlock(&mut); 
	} 
}

void do_another_thing(int* pnum_times) {
	int i, k, work;
  
	for (i = 0; i < 50; i++) {
		// pthread_mutex_lock(&mut); 
		printf("doing another thing\n");
		work = *pnum_times;
		printf("counter = %d\n", work);
		work++; /* increment, but not write */
		for (k = 0; k < 500000; k++); /* long cycle */
		*pnum_times = work; /* write back */
		// pthread_mutex_unlock(&mut); 
	} 
}

void do_wrap_up(int counter) {
	int total;
	printf("All done, counter = %d\n", counter);
}

```



```
[zybc@writer lab5]$ ./a.out 
doing one thing
counter = 0
doing one thing
counter = 1
doing one thing
counter = 2
doing one thing
counter = 3
doing one thing
counter = 4
doing one thing
counter = 5
doing one thing
counter = 6
doing one thing
counter = 7
doing one thing
counter = 8
doing one thing
counter = 9
doing one thing
counter = 10
doing one thing
counter = 11
doing one thing
counter = 12
doing one thing
counter = 13
doing one thing
counter = 14
doing one thing
counter = 15
doing one thing
counter = 16
doing one thing
counter = 17
doing one thing
counter = 18
doing one thing
counter = 19
doing one thing
counter = 20
doing one thing
counter = 21
doing one thing
counter = 22
doing one thing
counter = 23
doing one thing
counter = 24
doing one thing
counter = 25
doing one thing
counter = 26
doing one thing
counter = 27
doing one thing
counter = 28
doing one thing
counter = 29
doing one thing
counter = 30
doing one thing
counter = 31
doing one thing
counter = 32
doing one thing
counter = 33
doing one thing
counter = 34
doing one thing
counter = 35
doing one thing
counter = 36
doing one thing
counter = 37
doing one thing
counter = 38
doing one thing
counter = 39
doing one thing
counter = 40
doing one thing
counter = 41
doing one thing
counter = 42
doing one thing
counter = 43
doing one thing
counter = 44
doing one thing
counter = 45
doing one thing
counter = 46
doing one thing
counter = 47
doing one thing
counter = 48
doing one thing
counter = 49
doing another thing
counter = 50
doing another thing
counter = 51
doing another thing
counter = 52
doing another thing
counter = 53
doing another thing
counter = 54
doing another thing
counter = 55
doing another thing
counter = 56
doing another thing
counter = 57
doing another thing
counter = 58
doing another thing
counter = 59
doing another thing
counter = 60
doing another thing
counter = 61
doing another thing
counter = 62
doing another thing
counter = 63
doing another thing
counter = 64
doing another thing
counter = 65
doing another thing
counter = 66
doing another thing
counter = 67
doing another thing
counter = 68
doing another thing
counter = 69
doing another thing
counter = 70
doing another thing
counter = 71
doing another thing
counter = 72
doing another thing
counter = 73
doing another thing
counter = 74
doing another thing
counter = 75
doing another thing
counter = 76
doing another thing
counter = 77
doing another thing
counter = 78
doing another thing
counter = 79
doing another thing
counter = 80
doing another thing
counter = 81
doing another thing
counter = 82
doing another thing
counter = 83
doing another thing
counter = 84
doing another thing
counter = 85
doing another thing
counter = 86
doing another thing
counter = 87
doing another thing
counter = 88
doing another thing
counter = 89
doing another thing
counter = 90
doing another thing
counter = 91
doing another thing
counter = 92
doing another thing
counter = 93
doing another thing
counter = 94
doing another thing
counter = 95
doing another thing
counter = 96
doing another thing
counter = 97
doing another thing
counter = 98
doing another thing
counter = 99
All done, counter = 100
```



без

pthread_mutex_lock(&mut);
pthread_mutex_unlock(&mut); 

```
[zybc@writer lab5]$ gcc mutex.c -lpthread
[zybc@writer lab5]$ ./a.out 
doing one thing
counter = 0
doing another thing
counter = 0
doing one thing
counter = 1
doing another thing
counter = 1
doing one thing
counter = 2
doing another thing
counter = 2
doing one thing
counter = 3
doing another thing
counter = 3
doing one thing
counter = 4
doing another thing
counter = 4
doing one thing
counter = 5
doing another thing
counter = 5
doing one thing
counter = 6
doing another thing
counter = 6
doing one thing
counter = 7
doing another thing
counter = 7
doing one thing
counter = 8
doing another thing
counter = 8
doing one thing
counter = 9
doing another thing
counter = 9
doing one thing
counter = 10
doing another thing
counter = 10
doing one thing
counter = 11
doing another thing
counter = 11
doing one thing
counter = 12
doing another thing
counter = 12
doing one thing
counter = 13
doing another thing
counter = 13
doing one thing
counter = 14
doing another thing
counter = 14
doing one thing
counter = 15
doing another thing
counter = 15
doing one thing
counter = 16
doing another thing
counter = 16
doing one thing
counter = 17
doing another thing
counter = 17
doing one thing
counter = 18
doing another thing
counter = 18
doing another thing
counter = 19
doing one thing
counter = 19
doing another thing
counter = 20
doing one thing
counter = 20
doing another thing
counter = 21
doing one thing
counter = 21
doing another thing
counter = 22
doing one thing
counter = 22
doing another thing
counter = 23
doing one thing
counter = 23
doing another thing
counter = 24
doing one thing
counter = 24
doing another thing
counter = 25
doing one thing
counter = 25
doing another thing
counter = 26
doing one thing
counter = 26
doing another thing
counter = 27
doing one thing
counter = 27
doing another thing
counter = 28
doing one thing
counter = 28
doing another thing
counter = 29
doing one thing
counter = 29
doing another thing
counter = 30
doing one thing
counter = 30
doing another thing
counter = 31
doing one thing
counter = 31
doing another thing
counter = 32
doing one thing
counter = 32
doing another thing
counter = 33
doing one thing
counter = 33
doing another thing
counter = 34
doing one thing
counter = 34
doing another thing
counter = 35
doing one thing
counter = 35
doing another thing
counter = 36
doing one thing
counter = 36
doing another thing
counter = 37
doing one thing
counter = 37
doing another thing
counter = 38
doing one thing
counter = 38
doing another thing
counter = 39
doing one thing
counter = 39
doing another thing
counter = 40
doing one thing
counter = 40
doing another thing
counter = 41
doing one thing
counter = 41
doing another thing
counter = 42
doing one thing
counter = 42
doing another thing
counter = 43
doing one thing
counter = 43
doing another thing
counter = 44
doing one thing
counter = 44
doing another thing
counter = 45
doing another thing
counter = 46
doing one thing
counter = 45
doing another thing
counter = 46
doing one thing
counter = 46
doing another thing
counter = 47
doing one thing
counter = 47
doing another thing
counter = 48
doing one thing
counter = 48
doing one thing
counter = 49
All done, counter = 50
```


