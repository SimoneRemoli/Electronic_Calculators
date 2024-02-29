#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include <stdint.h>

#define fflush(stdin) while ((getchar()) != '\n')
#define MAXLINE 5000
#define MAXLEN 1000
void writelines(char**, int);

int main(int argc, char **argv)
{
	int num_arrays = 5; 	//number of arrays you want.
	char **array_di_pointers_a_carattere;
	char buffer[1024];
	array_di_pointers_a_carattere = (char**)malloc(sizeof(char *)*num_arrays);
	for(int i=0; i<num_arrays;i++)
	{
	    array_di_pointers_a_carattere[i] = (char*)malloc(sizeof(char) * 1024);
	    if(fgets(buffer,1024,stdin))
	    	buffer[strcspn(buffer, "\n")] = 0;
	    memcpy(array_di_pointers_a_carattere[i], buffer, 1024);
	}
	writelines(array_di_pointers_a_carattere, num_arrays);
	return 0;
}
void writelines(char *lineptr[], int nlines)
{
	while(nlines-- >0)
		printf("Valore stringa = %s \n", *lineptr++);
}