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

void load(void*);

int main(int argc, char **argv)
{
	int a = 10;
    float b = 20.5;
    char c = 'Z';
    int vet[10];
    char buffer[1024];

    if(fgets(buffer, 1024, stdin))
    	buffer[strcspn(buffer, "\n")] = 0;

    void**array_poin = (void**)malloc(sizeof(void*)*5);
    array_poin[0] = &a;
    array_poin[1] = &b;
    array_poin[2] = &c;
    array_poin[3] = &vet[0];
    array_poin[4] = &buffer[0];

    printf("Valore di a = %d \n", *(int*)array_poin[0]);
    printf("Valore di b = %f \n", *(float*)array_poin[1]);
    printf("Valore di c = %c \n", *(char*)array_poin[2]);
    printf("Valore della stringa = %s \n", (char*)array_poin[4]);
    load(array_poin[3]);

    for(int i=0;i<10;i++)
    {
    	printf("valore di vet[%d] = %d \n", i, vet[i]);
    }
	
	return 0;
}
void load(void* arg)
{
	int *ptr = arg;
	for(int i=0;i<10;i++)
		*(ptr + i) = i + 1;
}

