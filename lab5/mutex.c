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
