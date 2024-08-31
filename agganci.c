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
il programma acquisisce una stringa e dopodichè crea un numero di nodi pari al numero di parole e li aggancia a modo di SLL
*/

struct nodo
{
	char parola[1024];
	struct nodo* next;
};


struct nodo* testa = NULL;
struct nodo* inserimento(struct nodo*,char*);
bool lista_vuota(struct nodo*);
struct nodo* crea_nodo(char*);
struct nodo* views(struct nodo*);
#define fflush(stdin) while ((getchar()) != '\n')

int main(int argc, char** argv)
{
	char buffer[1024];
	char *token;
	if(fgets(buffer,1024,stdin))
		buffer[strcspn(buffer,"\n")] = 0;
	printf("Hai inserito = %s \n", buffer);

	token = strtok(buffer," ");
	while(token!=NULL)
	{
		//printf("Token = %s \n", token);

		testa = inserimento(testa,token);

		token = strtok(NULL, " ");
	}
	testa = views(testa);

	return 0;
}
struct nodo* views(struct nodo* testa)
{
	struct nodo* temp = testa;
	while(temp!=NULL)
	{
		printf(" %s \n", temp->parola);
		temp = temp->next;
	}
}
struct nodo* inserimento(struct nodo* testa, char *valore)
{
	struct nodo* nuovo_nodo;
	struct nodo* temp = NULL;
	struct nodo* prec = NULL;
	nuovo_nodo = crea_nodo(valore);
	if(lista_vuota(testa))
	{
		testa = nuovo_nodo;
	}
	else
	{
		temp = testa;
		while(temp!=NULL)
		{
			prec = temp;
			temp = temp->next;
		}
		prec->next = nuovo_nodo;
	}
	return testa;
}

bool lista_vuota(struct nodo* testa)
{
	if(testa==NULL)
		return true;
	return false;
}
struct nodo* crea_nodo(char* valore)
{
	struct nodo* nuovo_nodo = malloc(sizeof(struct nodo));
	strncpy(nuovo_nodo->parola, valore,1024);
	nuovo_nodo->next = NULL;
	return nuovo_nodo;
}
