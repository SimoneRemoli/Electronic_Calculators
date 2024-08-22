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

/*
	Scrivere un programma in linguaggio C che esegue la fusione di due vettori di interi ordinati in modo crescente.
	Il programma deve eseguire le seguenti operazioni:

	1.	Leggere due vettori di N interi. Il numero N viene inserito dall’utente ed è minore di
		20. I due vettori possono avere lunghezza diversa. I due vettori si suppongono già
		ordinati in maniera crescente.
	2.  Creare un terzo vettore di lunghezza pari alla somma delle lunghezze dei due vettori
		dati. Il vettore dovrà contenere i numeri contenuti nei due vettori di partenza. I
		numeri nel vettore devono essere ordinati in modo crescente.
	3.  Stampare il vettore generato.

			Ad esempio, si assuma che siano stati inseriti i due vettori
						1 6 15 20 25
						2 8 18 19.
			Il programma dovrà visualizzare la sequenza 1 2 6 8 15 18 19 20 25

*/

void load(void*,int);
void views(void*,int);
void sort(void*,int);
void swap(int*,int*);
void operation(void*,void*,void*,int,int);


int main(int argc, char **argv)
{
	//------------------------------------------
	void** uso = (void**)malloc(sizeof(void*)*3); //|
	int **double_array = malloc(sizeof(int)*6);   //|
	//------------------------------------------

	double_array[2] = malloc(sizeof(int)*1);
	double_array[3] = malloc(sizeof(int)*1);



	printf("Inserisci la dimensione del primo vettore: ");
	scanf("%d", &*(double_array[2]+0));
	printf("Inserisci la dimensione del secondo vettore: ");
	scanf("%d", &*(double_array[3]+0));
	int dim;
	dim = double_array[3][0] + double_array[2][0];

	//printf("vALORE SOMMA = %d \n", *(double_array[4] + 0));


	double_array[0] = malloc(sizeof(int)*(*(double_array[2]+0)));
	double_array[1] = malloc(sizeof(int)*(*(double_array[3]+0)));
	double_array[5] = malloc(sizeof(int)*dim);



	uso[0] = &double_array[0][0]; //uso[0] è un void* (possiamo fare qualsiasi cosa)
	uso[1] = &double_array[1][0]; //uso[1] è un void* (possiamo fare qualsiasi cosa)
	uso[2] = &double_array[5][0];


 // oppure int *ptr = &double_array[0][0]; da passare come primo parametro in load anche se accetta void*


	for(int i=0;i<2;i++) load(uso[i],double_array[i+2][0]);
	for(int i=0;i<2;i++) views(uso[i],double_array[i+2][0]);
	printf("Array ordinati: \n");
	for(int i=0;i<2;i++) sort(uso[i],double_array[i+2][0]);
	for(int i=0;i<2;i++) views(uso[i],double_array[i+2][0]);
	operation(uso[0],uso[1],uso[2],double_array[2][0],double_array[3][0]);


	return 0;
}
void operation(void* ptrone, void* ptrtwo, void* ptrt, int dim1, int dim2)
{
	int* ptr1 = ptrone;
	int* ptr2 = ptrtwo;
	int* ptr_tot = ptrt;

	for(int i=0;i<dim1;i++)
		*(ptr_tot + i) = *(ptr1 + i);
	for(int i=dim1,j=0;i<dim2+dim1;i++,j++)
			*(ptr_tot + i) = *(ptr2 + j);

	printf("Vettore totale: \n");
	views(ptr_tot,dim1+dim2);
	sort(ptr_tot,dim1+dim2);
	printf("Vettore ordinato: \n");
	views(ptr_tot,dim1+dim2);
	return;

}



void swap(int *i,int*j)
{
	int temp;
	temp = *i;
	*i = *j;
	*j = temp;
	return;
}
void sort(void *ptr, int dim)
{
	puts(" ");
	int *array = ptr;
	for(int i=0;i<dim;i++)
	{
		for(int j=i+1;j<dim;j++)
		{
			if(array[i]>array[j])
			{
				swap(&array[i],&array[j]);
			}
		}
	}
	return;
}
void views(void* ptr, int dim)
{
	int* array = ptr;
	puts(" ");
	for(int i=0;i<dim;i++)
	{
		printf(" %d ",*(array + i));
	}
	printf("\n");
	puts(" ");
	puts(" ");
	return;

}

void load(void* ptr,int dim)
{
	int *array = ptr,val=0;	
	for(int i=0;i<dim;i++)
	{
		scanf("%d",&val);
		* (array + i ) = val;

	}

	puts(" ");
	return;

}