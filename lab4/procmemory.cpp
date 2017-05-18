/* Program to display address information about the process */
/* Adapted from Gray, J., program 1.4 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>

/* Below is a macro definition */
#define SHW_ADR(ID,I) (printf("ID %s \t is at virtual address: %8p\n", ID, &I)) 

extern int etext, edata, end; /* Global variables for process memory */

const char* cptr = "This message is output by the function showit()"; /* Static */
char  buffer1[25];

int showit(const char* p); /* Function prototype */

int main() {
	int i = 0; /* Automatic variable */
	// fork();

	/* Printing addressing information */
	printf("\nAddress etext: %8p\n", &etext);
	printf("Address edata: %8p\n", &edata);
	printf("Address end  : %8p\n", &end);

	SHW_ADR("main", main);
	SHW_ADR("showit", showit);
	SHW_ADR("cptr", cptr);
	SHW_ADR("buffer1", buffer1);
	SHW_ADR("i\t", i);
	strcpy(buffer1, "A demonstration\n");   /* Library function */
	write(1, buffer1, strlen(buffer1) + 1); /* System call */
	showit(cptr);

	return 0;
}


/* A function follows */
int showit(const char* p) {
	char* buffer2;
	SHW_ADR("buffer2", buffer2);

	if ((buffer2 = (char*) malloc ((unsigned) (strlen(p) + 1))) != NULL) {
   		printf("Alocated memory at %p\n", buffer2);
   		strcpy(buffer2, p);    /* copy the string */
   		puts(buffer2); 		/* Didplay the string */
   		free(buffer2);         /* Release location */
   	} else {
   		printf("Allocation error\n");
   		exit(1);
   	}
} 

