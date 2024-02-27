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

void accesso(void*);


int main(int argc, char **argv)
{
	int ***general = (int***)malloc(sizeof(int**)*2);
	for(int i=0;i<2;i++)
	{
		general[i] = (int**)malloc(sizeof(int*)*5); //ciascuna base ha 5 elementi 
	}

	int a = 0;	int f = 5;
	int b = 1;	int g = 6;
	int c = 2;	int h = 7;
	int d = 3;	int i = 8;
	int e = 4;	int l = 9;

	general[0][0] = &a;	general[1][0] = &f; //singoli pointer
	general[0][1] = &b;	general[1][1] = &g;
	general[0][2] = &c;	general[1][2] = &h;
	general[0][3] = &d;	general[1][3] = &i;
	general[0][4] = &e;	general[1][4] = &l;
	//settato la base 0 e la base 1 

	printf("Valore di c = %d \n", *general[0][2]);
	for(int i=0;i<5;i++)
		accesso(general[0][i]);
	puts(" ");
	for(int i=0;i<5;i++)
		accesso(general[1][i]);


	
	return 0;
}
void accesso(void *arg)
{
	int value = *(int*)arg;
	printf("Valore di value = %d \n", value);
}
