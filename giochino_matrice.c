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

/*Realizzare un programma che abbia una griglia matriciale NxN, e l'utente deve provare ad indovinare una lettera in una certa 
posizione , se giusta la matrice si ingradisce ancora di più fino ad un massimo di 3 livelli.
Nel livello 1 la matrice è 3x3.
Nel livello 2 la matrice è 5x5
Nel livello 3 la matrice è 7x7.
*/
void load(char*,int);
void views(char*,int);
int play(char*,int);
int check = 0;
bool fine = false;

#define fflush(stdin) while ((getchar()) != '\n')

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	int dimensione_matrice = 3;
	/*char* matrice = malloc(sizeof(char)*dimensione_matrice*dimensione_matrice);
	load(matrice,dimensione_matrice);
	views(matrice,dimensione_matrice);*/


	while(fine!= true) 
	{
		char* matrice = malloc(sizeof(char)*dimensione_matrice*dimensione_matrice);
		load(matrice,dimensione_matrice);
		dimensione_matrice = play(matrice,dimensione_matrice);
	}

	return 0;
}
int play(char* matrice,int dimensione_matrice)
{
	
		char prova;
		int riga=0,colonna=0;
		printf("Scegli riga: ");
		scanf("%d",&riga);
		printf("Scegli colonna: ");
		scanf("%d",&colonna);
		fflush(stdin);
		printf("Tira ad indovinare il carattere: ");
		scanf("%c",&prova);

		if(*(matrice + riga*dimensione_matrice + colonna)==prova)
		{
			printf("Ok, preso. [%d] Tentativo riuscito. \n",check+1);
			check = check + 1;
			printf("La matrice infatti era la seguente:\n");
			views(matrice,dimensione_matrice);

		}
		else
		{
			printf("Hai perso [*] - Mi dispiace [*] \n");
			printf("Peccato - guarda dove hai sbagliato: \n");
			views(matrice,dimensione_matrice);

			fine = true;
		}
		if(check==3)
		{
			printf("Hai vinto il gioco[*] \n");
			fine = true;
		}
	
	return dimensione_matrice+2;
}
void load(char*matrice, int dimensione_matrice)
{
	for(int i=0;i<dimensione_matrice;i++)
	{
		for(int j=0;j<dimensione_matrice;j++)
		{
			*(matrice+i*dimensione_matrice+j) = rand()%26+65;
		}
	}
	return;
}
void views(char*matrice,int dimensione_matrice)
{
	puts(" ");
	for(int i=0;i<dimensione_matrice;i++)
	{
		for(int j=0;j<dimensione_matrice;j++)
		{
			printf("  %c ", *(matrice+i*dimensione_matrice+j));
		}
		printf("\n");
	}
	return;
}