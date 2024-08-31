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
Lista che ha internemente un nome, cognome, età e tutti i soprannomi che gli hanno dato nel corso della sua vita.
Il programma deve prevedere un inserimento, una visualizzazione, una cancellazione (dato un nome) e un ordinamento, e creazione
di una lista inversa.
*/

struct nodo
{
	char nome[1024];
	char cognome[1024];
	int eta;
	char **soprannomi;
	int numero_soprannomi;
	struct nodo* next;
};

#define fflush(stdin) while ((getchar()) != '\n')

struct nodo* inserimento(struct nodo*);
struct nodo* views(struct nodo*);
bool lista_vuota(struct nodo*);
struct nodo* crea_nodo(void);
struct nodo* cancellazione(struct nodo*);
struct nodo* cancellazione_eta(struct nodo*);
struct nodo* ordinamento(struct nodo*);
struct nodo* inversione(struct nodo*);
struct nodo* testa = NULL;



int main(int argc, char** argv)
{
	bool fine = false;
	int scelta = 0;
	do{
		puts(" ");
		puts("1. Inserimento lista.");
		puts("2. Visualizzazione lista.");
		puts("3. Cancellazione per nome.");
		puts("4. Ordinamento lista.");
		puts("5. Crea lista inversa.");
		puts("6. Cancellazione di tutti gli utenti con età superioriore ad un certo valore.");
		printf(">>");
		scanf("%d", &scelta);

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
				testa = cancellazione(testa);
			}break;
		case 4:
			{
				testa = ordinamento(testa);
			}break;
		case 5:
			{
				testa = inversione(testa);
			}break;
		case 6:
			{
				testa = cancellazione_eta(testa);
			}break;
		default:
			{
				fine = true;
				fputs("Aborted [*] ", stdout);
			}
		}

	}while(fine!=true);

	return 0;
}

struct nodo* inversione(struct nodo* testa)
{
	struct nodo* prec = NULL;
	struct nodo* temp = testa;
	struct nodo* app = NULL;

	while(temp!=NULL)
	{
		app = temp->next;


		temp->next = prec;


		prec = temp;
		temp = app;


	}
	return prec;
}


struct nodo* cancellazione_eta(struct nodo* testa)
{
	int eta_da_eliminare;
	bool fine = true,controllo=false;
	struct nodo* appoggio = NULL;
	struct nodo* prec = NULL;
	struct nodo* temp = testa;
	struct nodo* app = NULL;

	printf("Inserisci il numero di età (verranno eliminati tutti gli utenti superiori a questa età): ");
	scanf("%d",&eta_da_eliminare);

	while(fine)
	{
		if(testa==NULL)
		{
			printf("Lista svuotata [*] \n");
			return testa;
		}
		if(testa->eta>eta_da_eliminare)
		{
			appoggio = testa;
			testa = testa->next;
			appoggio->next = NULL;
			free(appoggio);
			fine = true;
		}
		else
			fine = false; //forzo l'uscita 
	}
	prec = testa;
	temp = testa->next;
	while(temp!=NULL)
	{
		if(temp->eta>eta_da_eliminare)
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
			prec = temp;
			temp = temp->next;
		}
	}
	return testa;
}

struct nodo* ordinamento(struct nodo* testa)
{
	struct nodo* i = testa;
	struct nodo* appoggio = malloc(sizeof(struct nodo));
	while(i!=NULL)
	{
		struct nodo* j=i->next;
		while(j!=NULL)
		{
			if(strcmp(i->cognome,j->cognome)>0)
			{
				strncpy(appoggio->nome,i->nome,1024);
				strncpy(appoggio->cognome,i->cognome,1024);
				appoggio->eta = i->eta;
				appoggio->numero_soprannomi = i->numero_soprannomi;
				appoggio->soprannomi = i->soprannomi;

				strncpy(i->nome,j->nome,1024);
				strncpy(i->cognome,j->cognome,1024);
				i->eta = j->eta;
				i->numero_soprannomi = j->numero_soprannomi;
				i->soprannomi = j->soprannomi;

				strncpy(j->nome,appoggio->nome,1024);
				strncpy(j->cognome,appoggio->cognome,1024);
				j->eta = appoggio->eta;
				j->numero_soprannomi = appoggio->numero_soprannomi;
				j->soprannomi = appoggio->soprannomi;
			}
			j = j->next;
		}
		i = i->next;
	}
	return testa;
}

struct nodo* cancellazione(struct nodo* testa)
{
	char nome_da_eliminare[1024];
	struct nodo* prec = NULL;
	struct nodo* temp = testa;
	struct nodo* appoggio = NULL;
	bool trovato = false;
	fflush(stdin);
	printf("Inserisci il nome da eliminare: ");
	if(fgets(nome_da_eliminare,1024,stdin))
		nome_da_eliminare[strcspn(nome_da_eliminare,"\n")] = 0;

	if(strcmp(testa->nome,nome_da_eliminare)==0)
	{
		appoggio = testa;
		testa = testa->next;
		appoggio->next = NULL;
		free(appoggio);
		trovato = true;
	}
	else
	{
		while((temp!=NULL)&&(trovato == false))
		{
			if(strcmp(temp->nome,nome_da_eliminare)==0)
			{
				trovato = true;
				prec->next = temp->next;
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




struct nodo* views(struct nodo* testa)
{
	struct nodo* temp = testa;
	printf("\n");
	while(temp!=NULL)
	{
		puts("----------------");
		printf("Nome: %s \n Cognome: %s \n",temp->nome,temp->cognome);
		printf("Età: %d \n", temp->eta);
		for(int i=0;i<temp->numero_soprannomi;i++)
		{
			printf("[%d] - Soprannome : %s \n", i+1, temp->soprannomi[i]);
		}
		temp = temp->next;
	}
	printf("\n");
	return testa;
}

bool lista_vuota(struct nodo* testa)
{
	if(testa==NULL)
		return true;
	return false;
}
struct nodo* crea_nodo()
{
	char nome[1024],cognome[1024],buffer[1024];
	int eta,num_sopran;
	fflush(stdin);
	printf("Inserisci nome: ");
	if(fgets(nome,1024,stdin))
		nome[strcspn(nome, "\n")] = 0;
	printf("Inserisci cognome: ");
	if(fgets(cognome,1024,stdin))
		cognome[strcspn(cognome,"\n")] = 0;
	printf("Inserisci età: ");
	scanf("%d", &eta);
	fflush(stdin);
	printf("Quanti soprannomi ha avuto %s in %d anni di vita? ", cognome,eta);
	scanf("%d", &num_sopran);
	char** soprann = (char**)malloc(sizeof(char*)*num_sopran);
	fflush(stdin);
	for(int i=0;i<num_sopran;i++)
	{
		printf("[%d] soprannome: ",i+1);
		soprann[i] = malloc(sizeof(char)*1024);
		if(fgets(buffer,1024,stdin))
			buffer[strcspn(buffer,"\n")] = 0;
		memcpy(soprann[i],buffer,1024);
	}
	struct nodo* nuovo_nodo = malloc(sizeof(struct nodo));
	strncpy(nuovo_nodo->nome, nome, 1024);
	strncpy(nuovo_nodo->cognome,cognome,1024);
	nuovo_nodo->eta=eta;
	nuovo_nodo->numero_soprannomi = num_sopran;
	nuovo_nodo->soprannomi = soprann;
	nuovo_nodo->next = NULL;
	return nuovo_nodo;
}
struct nodo* inserimento(struct nodo* testa)
{
	struct nodo* nuovo_nodo = NULL;
	struct nodo* temp = NULL;
	struct nodo* prec = NULL;
	nuovo_nodo = crea_nodo();
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