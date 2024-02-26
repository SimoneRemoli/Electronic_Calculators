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
#define MAX_DIM 1000000
void load(void*);
void views(void*);
long long int search(void*, int);
int cont = 0;
int main(int argc, char **argv)
{
	int val = 55;
	int ameba = 33;
	int *ptr;
	int *ptr_array = malloc(sizeof(int)*10);
	printf("Massima dimensione dell'array = %lu \n", SIZE_MAX);
	int array[MAX_DIM];
	printf("Valore dell'indirizzo di val = %p \n", &val);
	for(int i=0;i<10;i++)
	{
		printf("[%d] - Indirizzo successivo = %p \t valore successivo = %d \n", i, &val+i, *(&val+i)); //avanti di 4 byte (della dimensione int va avanti)
		if(i==8)
		{
			ptr = &val+i;
		}
	}
	printf("Valore dereferenziato = %d  \n", *(&val));
	printf("Il puntatore punta a questo indirizzo = %p \n", ptr);
	printf("L'indirizzo del puntatore = %p \n", &ptr);
	printf("Valore della ottava posizione = %d \n", *ptr);
	printf("\n");
	int **pttr = &ptr;
	printf("**(&ptr) = %d \n", **(&ptr)); // = **(pttr) = *(ptr) = ***(ptttr)
	printf("***(&pttr) = %d \n", ***(&pttr));
	printf("*(&ptr) = %p \n", *(&ptr)); // = *pttr = andare all'indirizzo del puntatore ptr e accedere con * significa prendere l'indirizzo a cui puntava ptr
	printf("  Il valore del doppio puntatore = %d  \n  l'indirizzo del doppio puntatore = %p  \n  l'indirizzo a cui punta il doppio puntatore = %p  l'indirizzo del puntatore precedente = %p\n", **(pttr),&pttr, *pttr, pttr);
	int ***ptttr = &pttr;
	printf("\n");
	printf("  Il valore del triplo puntatore = %d  \n  l'indirizzo del triplo puntatore = %p  \n  l'indirizzo a cui punta il triplo puntatore = %p  l'indirizzo del puntatore precedente = %p\n", ***ptttr, &ptttr, **ptttr, ptttr);
	printf("Valore = %p \n", *ptttr );
	printf("\n");
	printf("-----\n");
	clock_t begin = clock();
	int **pointer;


	pointer = (int**)malloc(sizeof(int*)*10); //10 puntatori singoli
	for(int i=0;i<10;i++) pointer[i] = &array[i*100000]; 


	load(array);
	/*for(int i=0;i<100000;i++)
	{
		printf("Valore dell'ottavo pointer [%d] = %d \n",i,*(pointer[8] + i)); //ci fornisce i valori che correntemente lui gestisce 
	}
	*/
	//views(array);
	puts(" ");
	long long int pos;
	char num[1024];
	while(cont<10)
	{
		if(cont==9)
			printf("Valore iniziale di partenza del [%d] puntatore = %d |  \t range[%d,%d] \n", cont, *(pointer[cont]), *(pointer[cont]), MAX_DIM);
		else
			printf("Valore iniziale di partenza del [%d] puntatore = %d |  \t range[%d,%d] \n", cont, *(pointer[cont]), *(pointer[cont]), *(pointer[(cont+1)])-1);
		cont++;
	}
	//printf("Inserisci valore da cercare:");
	//fgets(num, 1024, stdin);
	int numero = 1000000;
	/*if((numero>=1)&&(numero<=100000))
		pos = search(pointer[0], numero); //i=1 1*100000
	if((numero>=100001)&&(numero<=200000))
		pos = search(pointer[1], numero);
	if((numero>=200001)&&(numero<=300000))
		pos = search(pointer[2], numero);
	if((numero>=300001)&&(numero<=400000))
		pos = search(pointer[3], numero);
	if((numero>=400001)&&(numero<=500000))
		pos = search(pointer[4], numero);
	if((numero>=500001)&&(numero<=600000))
		pos = search(pointer[5], numero);
	if((numero>=600001)&&(numero<=700000))
		pos = search(pointer[6], numero);
	if((numero>=700001)&&(numero<=800000))
		pos = search(pointer[7], numero);
	if((numero>=800001)&&(numero<=900000))
		pos = search(pointer[8], numero);
	if(numero>=900001)
		pos = search(pointer[9], numero);*/
	for(int i=0;i<10;i++)
	{
		if(i!=9)
		{
			if((numero>=(i*100000)+1)&&(numero<=(i+1)*100000))
				pos = search(pointer[i], numero);
		}
		else //last index 
		{
			if(numero>=(i*100000)+1)
				pos = search(pointer[i], numero);
		}
	}
	printf("a frocio");

	//pos = search(array,numero);



	//long long int pos = search(array, numero);
	//if(pos)
		printf("Valore %s trovato. Posizione = %lld \n",  pos ? "perfettamente":"non", pos );
	//else
		//puts("Valore non trovato [*]");

	//pointer[0] = &ptr_array;
	/*pointer[0] = &ameba;
	printf("Valore del pointer = %d \n ", *(pointer[0]));
	printf("L'indirizzo del pointer = %p \n", &(pointer[0]));
	printf("L'indirizzo a cui punta il pointer = %p \n", pointer[0]);
	*/

	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("Valore del tempo = %f \n", time_spent);
	return 0;
}
void load(void* arg)
{
	int *ptr = arg;
	for(long long int i=0;i<MAX_DIM; i++)
	{
		*(ptr + i) = i+1;
	}
}
void views(void* arg)
{
	int *ptr = arg;
	for(long long int i=0;i<MAX_DIM;i++)
	{
		printf(" \t Valore [%lld] = %d \n", i+1, *(ptr + i));
	}
}
long long int search(void* arg, int value) //non sequenziale - Accesso Indicizzato
{
	int *ptr = arg;
	for(long long int i = 0; i<100000; i++)
	{
		if(*(ptr + i) == value)
		{
			return i;
		}
	}
	return 0;

}
/*
long long int search(void* arg, int value) //non indicizzata - Accesso Sequenziale
{
	int *ptr = arg;
	for(long long int i = 0; i<MAX_DIM;i++)
	{
		if(*(ptr + i) == value)
		{
			return i;
		}
	}
	return 0;
}*/