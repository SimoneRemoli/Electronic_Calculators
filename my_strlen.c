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


int lunghezza_stringa(char*);




int main(int argc, char **argv)
{
	//char *mamma = "mamma";
	char mamma[1024];
	if(fgets(mamma, 1024, stdin))
	{
		mamma[strcspn(mamma, "\n")] = 0;
	}
	printf("Dimensione di mamma = %d \n", lunghezza_stringa(mamma));	
	return 0;
}
int lunghezza_stringa(char *s)
{
	int count = 0;
	char *p = s;
	while(*p!='\0')
	{
		p++;
		count = count + 1;
	}
	return count;
}
