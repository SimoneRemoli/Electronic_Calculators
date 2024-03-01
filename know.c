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

void* worker(int, void*,void*);
union Example 
{
   void* (*ptr)(int, void*, void*);
   int a;
   int b;
   float c;
};
void* general(int, void*,void*,void*(*)(int, void*,void*));


int main()
{
	union Example istanza1, istanza2, istanza3, istanza4, istanza5;
	
	istanza1.c = 1001.54;  //float
	istanza2.c = 23.4;	   //float 

	istanza4.a = 23;	  //int
	istanza5.b = 23;	  //int

	void **arrpoi = (void**)malloc(sizeof(void*)*4);
	arrpoi[0] = &istanza1.c;
	arrpoi[1] = &istanza2.c;
	arrpoi[2] = &istanza4.a;
	arrpoi[3] = &istanza5.b;

	int res = *(int*)general(0, arrpoi[2],arrpoi[3], istanza3.ptr);
	printf("Risultato = %d \n", res);

	float res1 = *(float*)general(1, arrpoi[0],arrpoi[1], istanza3.ptr);
	printf("Risultato = %f \n", res1);


   return 0;
}

void* worker(int opcode, void* a, void* b)
{
	int resint = 0;
	float resflo;
	void* ptrint = &resint;
	void* ptrflo = &resflo;
	switch(opcode)
	{
	case 0:
		{
			 resint = *(int*)a + *(int*)b;
		}break;
	case 1:
		{
			resflo = *(float*)a - *(float*)b;
		}break;
	}

	if(opcode == 0)
		return ptrint;
	else
		return ptrflo;
}
void* general(int opcode, void* a, void* b, void*(*ptrf)(int, void*,void*))
{
	ptrf = worker;
	int risultato = 0;
	float risultato_f = 0;
	void* ri = &risultato;
	void* rf = &risultato_f;
	switch(opcode)
	{
	case 0:
		{
			printf("Opcode = %d addizione tra int.\n", opcode);
			printf("Valore di a = %d \n", *(int*)a);
			printf("Valore di b = %d \n", *(int*)b);
			risultato = *(int*)(*ptrf)(opcode,a,b);
		}break;
	case 1:
		{
			printf("Opcode = %d sottrazione tra float.\n", opcode);
			printf("Valore di a = %f \n", *(float*)a);
			printf("Valore di b = %f \n", *(float*)b);
			risultato_f = *(float*)(*ptrf)(opcode, a, b);
		}
	}
	if(opcode == 0)
		return ri;
	else
		return rf;
}
