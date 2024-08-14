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
#include <math.h>
void funzione(int*);
void funzione2(char*);

int main(int argc, char **argv)
{
	int b = 1024;
	int h[5] = {0,1,2,3,4};
	int *ptr = &h[0];
	int *ptr2 = &b;
	char buffer[1024] = "mammina mia";
	//char *buffer = "mammina mia"; //uguale
	//char *bu = &buffer[0]

	funzione(ptr);
	funzione2(buffer);
	return 0;
}
void funzione(int *ptr)
{
	int valore = 5;
	while(valore--)
		printf("Valore = %d \n", *ptr++);
	return;
}
void funzione2(char *ptr)
{
	int valore  = strlen(ptr);
	while(valore--)
		printf("Valore carattere = %c \n", *ptr++);
}