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
-creazione lista-
-views-
-ordinamento per cognome-
-cancellazione per cognome-
-cancella tutti i cognomi che cominciano per una data lettera-
-creare la lista inversa-
*/
struct nodo
{
	char nome[1024];
	char cognome[1024];
	struct nodo* next;
};
struct nodo* app = NULL;
struct nodo* testa = NULL;

#define fflush(stdin) while ((getchar()) != '\n')
bool lista_vuota(struct nodo* testa);
struct nodo* crea_nodo(void);
struct nodo* inserimento(struct nodo*);
struct nodo* views(struct nodo*);
struct nodo* ordina(struct nodo*);
struct nodo* cancellazine(struct nodo*);
struct nodo* cancellazione_data_lettera(struct nodo*);
struct nodo* inverti_lista(struct nodo*);

int main(int argc, char** argv)
{
	bool fine = false;
	int scelta = 0;
	do{
		puts(" ");
		puts("1. Inserimento [*]");
		puts("2. Visualizzazione [*]");
		puts("3. Ordina per cognome [*]");
		puts("4. Cancellazione per cognome [*]");
		puts("5. Cancellazione di tutti gli studenti che cominciano con una data lettera del cognome [*]");
		puts("6. Crezione della lista inversa [*] ");
		printf(">>");
		scanf("%d",&scelta);

		switch(scelta)
		{
		case 1:
			{
				testa = inserimento(testa);
			}break;
		case 2:
			{
				testa = views(testa);
			}break;
		case 3:
			{
				testa = ordina(testa);
			}break;
		case 4:
			{
				testa = cancellazine(testa);
			}break;
		case 5:
			{
				testa = cancellazione_data_lettera(testa);
			}break;
		case 6:
			{
				testa = inverti_lista(testa);
			}break;
		default:
			{
				fputs("Aborted [*]. ",stdout);
				return 0;
			}
		}
	}while(fine!=true);
	return 0;
}



struct nodo* cancellazione_data_lettera(struct nodo* testa)
{
	char car;
	bool controllo = false;
	struct nodo* vecchio = NULL;
	struct nodo* temp = testa;
	struct nodo* prec = NULL;
	struct nodo* app = NULL;
	fflush(stdin);
	printf("Inserisci una lettera iniziale di un cognome: ");
	scanf("%c", &car);
	printf("\n Hai digitato = '%c' .\n", car);

	bool check = true;

	while(check)
	{
		if(testa->cognome[0]==car)
		{
			vecchio = testa;
			testa = testa->next;
			vecchio->next = NULL;
			free(vecchio);
			check = true;
		}
		else
			check = false;
	}

	prec = testa;
	temp = testa->next;

	while(temp!=NULL)
	{
		if(temp->cognome[0]==car)
		{
			controllo = true;
			prec->next = temp->next;
			app = temp->next;
			temp->next = NULL;
			free(temp);
			temp = app;
		}
		else
			controllo = false;
		if(controllo==false)
		{
			prec =  temp;
			temp = temp->next;
		}
	}

	return testa;

}

struct nodo* crea_nodo()
{

	char nome[1024],cognome[1024];

	printf("Inserisci cognome: \n");
	fflush(stdin);
	if(fgets(cognome, 1024, stdin))
		cognome[strcspn(cognome,"\n")] = 0;
	printf("Inserisci nome: \n");
	if(fgets(nome,1024,stdin))
		nome[strcspn(nome,"\n")] = 0;

	struct nodo* nuovo_nodo = malloc(sizeof(struct nodo));
	strncpy(nuovo_nodo->cognome,cognome,1024);
	strncpy(nuovo_nodo->nome,nome,1024);

	nuovo_nodo->next = NULL;
	return nuovo_nodo;
}

bool lista_vuota(struct nodo* testa)
{
	if(testa==NULL)
		return true;
	return false;
}


struct nodo* inverti_lista(struct nodo* testa)
{
    struct nodo *curr = testa, *prev = NULL, *next;
   
    while (curr != NULL) 
    {
        next = curr->next;
        curr->next = prev;

        prev = curr;
        curr = next;
    }
    return prev;
}
struct nodo* cancellazine(struct nodo* testa)
{
	struct nodo* temp = testa;
	struct nodo* prec=NULL;
	struct nodo* vecchio=NULL;
	char cognome[1024];
	bool trovato = false;
	printf("Inserisci il cognome da cancellare: ");
	fflush(stdin);
	if(fgets(cognome,1024,stdin))
		cognome[strcspn(cognome,"\n")] = 0;


	if(strcmp(testa->cognome,cognome)==0)
	{
		vecchio = testa;
		testa = testa->next;
		vecchio->next = NULL;
		free(vecchio);
		trovato = true;
	}
	else
	{
		while((temp!=NULL)&&(trovato==false))
		{
			if(strcmp(temp->cognome,cognome)==0)
			{
				trovato = true;
				prec->next = temp->next;
				temp->next = NULL;
				free(temp);
			}
			else
			{
				prec = temp;
				temp = temp->next;
			}

		}
	}
	return testa;
}
struct nodo* ordina(struct nodo* testa)
{
	struct nodo* i = testa;
	struct nodo* temporaneo = malloc(sizeof(struct nodo));

	while(i!=NULL)
	{
		struct nodo* j = i->next;
		while(j!=NULL)
		{
			if(strcmp(i->cognome,j->cognome)>0)
			{
				strncpy(temporaneo->nome, i->nome, 1024);
				strncpy(temporaneo->cognome, i->cognome,	1024);

				strncpy(i->nome, j->nome, 1024);
				strncpy(i->cognome, j->cognome, 1024);

				strncpy(j->nome, temporaneo->nome, 1024);
				strncpy(j->cognome, temporaneo->cognome, 1024);
			}
			j = j->next;
		}
		i = i->next;
	}
	return testa;
}




struct nodo* views(struct nodo* testa)
{
	struct nodo* appoggino = testa;
	while(appoggino!=NULL)
	{
		printf("---------------------------\n");
		printf("Cognome = %s \n", appoggino->cognome);
		printf("Nome = %s \n", appoggino->nome);
		appoggino = appoggino->next;
	}
	return testa;
}


struct nodo* inserimento(struct nodo* testa)
{

	struct nodo* nuovo_nodo = NULL;
	struct nodo* temp = NULL;
	struct nodo* prec = NULL;
	nuovo_nodo = crea_nodo();

	if(lista_vuota(testa)==true)
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
		prec ->next = nuovo_nodo;
	}
	return testa;

}