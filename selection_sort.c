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

void swap(int*,int*);


int main(int argc, char **argv)
{
	int temp;
	int h[5] = {5,3,1,2,4};
	for(int i=0;i<5;i++)
	{
		for(int j = i+1;j<5;j++)
		{
			if(h[i]>h[j])
			{
				swap(&h[i],&h[j]);
				/*temp = h[i];
				h[i] = h[j];
				h[j] = temp;*/
			}
		}
	}
	for(int i=0;i<5;i++)
	{
		printf("Val = %d \n", *(h+i));
	}
	
	return 0;
}
void swap(int* a, int* b)
{
	int temp;
	temp = *a;
	*a = *b; 
	*b = temp;
}
