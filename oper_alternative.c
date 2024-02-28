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

int add(int, int);
int sub(int,int);
void* operazione(int, int, int(*)(int,int), char*);
int main(int argc, char **argv)
{
	int (*ptrf)(int,int);
	int somma, sottrazione;
	somma = *(int*)operazione(3,2,ptrf, "somma");
	sottrazione = *(int*)operazione(3,2,ptrf,"sottrazione");
	printf("La somma = %d \n", somma);
	printf("La sottrazione = %d \n", sottrazione);
	return 0;
}
int add(int a, int b)
{
	return a+b;
}
int sub(int a, int b)
{
	return a-b;
}
void* operazione(int a,int b, int (*p)(int,int), char *op)
{
	int res = 0;
	void* risultato = &res;
	if(strcmp(op, "somma")==0)
	{
		p = add;
		res = (*p)(a,b); // come se fosse p(a,b);
	}	
	if(strcmp(op, "sottrazione")==0)
	{
		p = sub;
		res = (*p)(a,b); // come se fosse p(a,b);
	}
	return risultato;

}