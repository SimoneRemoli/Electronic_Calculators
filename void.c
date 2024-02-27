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

void load(void*,int);
void views(void*,void*);


int main(int argc, char **argv)
{
	int dim = 10;
	int *ptr = malloc(sizeof(int)*dim);

	void* ptr_dim = &dim;
	load(ptr,*(int*)ptr_dim);
	views(ptr, ptr_dim);


	


	return 0;
}

void load(void *arg, int i)
{
	printf(" ");
	int *p = arg;
	*(p + i) = 1;
	if(i == -1)
		return;
	else
		load(arg,i-1);
}
void views(void* arg, void* arg2)
{
	printf("\n");
	int *scan = arg;
	int dimensione = *(int*)arg2;
	for(int i=0;i<dimensione;i++)
	{
		printf(" %d \n", *(scan + i ));
	}
}