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

/*
Realizzare un programma in C che accetti in input un numero
di interi, dove è specificato dall'utente come parametro a riga di comando.
Gli interi vengono memorizzati all'interno di un vettore. Il programma restituisce in output il numero di bit
impostati a 1 nel vettore di interi inseriti dall'utente.
*/
int numero;
void view(int *);
void load(int *);
void to_binary(int value);
void count_ones(int);
int count = 1;
long contatore_uni = 0;

int main(int argc, char **argv)
{
	printf(">>");
	scanf("%d", &numero);
	int *ptr = malloc(sizeof(int)*numero);
	load(ptr);
	view(ptr);
	free(ptr);
	printf("Numero di uni = %ld \n", contatore_uni);

	return 0;
}
void load(int *ptr)
{
	char buffer[1024];
	int val;
	for(int i=0;i<numero;i++)
	{
		printf("Inserisci il valore [%d]:",i+1);
		scanf("%d", &val);
		*(ptr + i) = val;
	}
	return;
}
void view(int *ptr)
{
	for(int i = 0;i<numero; i++)
	{
		printf("%d", *(ptr + i));
		to_binary(*(ptr + i));
		count_ones(*(ptr + i));
	}
	return;
}
void to_binary(int value)
{
	int *ptr1 = malloc(sizeof(int)*64);
	for(int i=0;i<64;i++)
		*(ptr1 + i) = 0;
	int i = 63; /* A 64 bit */
	while(value != 0)
	{
		if(value %2 == 0)
			*(ptr1 + i) = 0;
		else
			*(ptr1 + i) = 1;
		value = value / 2;
		i = i - 1;
	}
	printf("[%d] item - ",count);
	for(int i=0;i<64;i++) printf("%d", *(ptr1 + i));
	printf("\n");
	count = count + 1;
	free(ptr1);
	return;
}
void count_ones(int value)
{
	while(value!=0) 
	{
		contatore_uni++;
		value &= value - 1;/*si sottrae 1 dal bit meno
							significativo di un numero binario positivo N
							e si esegue l'operazione logica AND sui numeri
							ed, il numero risultante ha un 1 in meno, 
							essendo stato eliminato quello meno significativo.*/
		printf("\t Value = %d \n", value);

		//value = 10 = 00000000000000000000000000001010 -
		//											  1
		//			   ---------------------------------
		//value=10-1 = 00000000000000000000000000001000
		//and ->  10   00000000000000000000000000001010
		//---------------------------------------------------
		//             00000000000000000000000000001000  -      = 8 count_uni = 1
		//											  1
		//			   ---------------------------------
		//value=8-1=   00000000000000000000000000000010
		//and ->  8    00000000000000000000000000001000
		//---------------------------------------------------
		//			   00000000000000000000000000000000       = 0, count_uni = 2		
	}
	return;
}
