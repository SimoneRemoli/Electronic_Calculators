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

/* 
Esame di programmazione C del 20 febbraio 2024 - Corso di Laurea in Ingegneria Informatica presso l'Università degli Studi di Roma Tor Vergata.
Sessione Invernale.
Corso erogato dal Prof.Ing. A.Pellegrini.
Studente: Simone Remoli.
Nome Corso: Calcolatori Elettronici.
CFU: 9.
*/
/*
											S P E C I F I C A 
Si vuole realizzare un'applicazione in C in grado di compilare un dizionario dei sinonimi. Le operazioni che si
chiede di supportare sono:
1. L'inserimento di una parola;
2. L'inserimento di un sinonimo di una parola;
3. La stampa di tutte le parole che iniziano con la stessa lettera;
4. Tutti i sinonimi di una parola inserita in input.
Il dizionario delle parole è organizzate in liste. Ciascuna lista contiene (in maniera non ordinata) tutte le parole
che iniziano con la stessa lettera. Tali liste sono raggiungibili mediante un vettore. Data una parola, tutti i
sinonimi sono anch'essi organizzati in una lista.
*/
#define fflush(stdin) while ((getchar()) != '\n')
int cont_parole = 0;
struct nodo_parola
{
	char parola[1024];
	struct nodo_parola* next;
};
struct nodo_sinonimi
{
	char sinonimo[1024][1024];
	int indice;
	struct nodo_sinonimi* next2;
};

struct nodo_parola* testa_parola = NULL;
struct nodo_sinonimi* testa_sinonimi = NULL;
struct nodo_parola* app1 = NULL;
struct nodo_sinonimi* app2 = NULL;
struct nodo_parola* inserisci_parola(struct nodo_parola*);
struct nodo_sinonimi* inserisci_sinonimo(struct nodo_sinonimi*);
struct nodo_parola* stampa_parola_che_iniziano_con_stessa_lettera(struct nodo_parola*);
void stampa_sinonimi_di_una_parola();

int main(int argc, char **argv)
{
	bool scelta = false;
	int choice = 0;
	do
	{
		printf("\n");
		puts("1. Inserisci una parola.");
		puts("2. Inserisci sinonimo.");
		puts("3. Stampa di tutte le parole che iniziano con la stessa lettera.");
		puts("4. Stampa di tutti i sinonimi di una parola inserita in input.");
		puts("5. Esci.");
		printf(">>");
		scanf("%d", &choice);
		switch(choice)
		{
		case 1:
			{
				testa_parola = inserisci_parola(testa_parola);
			}break;
		case 2:
			{
				testa_sinonimi = inserisci_sinonimo(testa_sinonimi);
			}break;
		case 3:
			{
				testa_parola = stampa_parola_che_iniziano_con_stessa_lettera(testa_parola);
			}break;
		case 4:
			{
				stampa_sinonimi_di_una_parola();
			}break;
		default:
			{
				scelta = true;
				printf("Aborted [*] \n");
			}
		}
	}while(scelta!= true);
	return 0;
}
struct nodo_parola* inserisci_parola(struct nodo_parola* testa_parola)
{
	char buffer_parola[1024];
	printf("Inserisci la parola: ");
	fflush(stdin);
	if(fgets(buffer_parola, 1024, stdin))
		buffer_parola[strcspn(buffer_parola, "\n")] = 0;
	struct nodo_parola* nuovo_nodo = malloc(sizeof(struct nodo_parola));
	strncpy(nuovo_nodo->parola, buffer_parola, 1024);
	if(testa_parola== NULL)
	{
		testa_parola = nuovo_nodo;
		app1 = testa_parola;
	}
	else
	{
		app1->next = nuovo_nodo;
		app1 = nuovo_nodo;
	}
	struct nodo_sinonimi* nuovo_nodo2 = malloc(sizeof(struct nodo_sinonimi)); //creo i nuovi nodi dei sinonimi e li aggancio
	nuovo_nodo2->indice = 0;
	if(testa_sinonimi == NULL)
	{
		testa_sinonimi = nuovo_nodo2;
		app2 = testa_sinonimi;
	}
	else
	{
		app2->next2 = nuovo_nodo2;
		app2 = nuovo_nodo2;
	}
	struct nodo_parola* temp1 = testa_parola;
	printf("\n");
	printf("\t ------------------------------------\n");
	printf("\t Parole attualmente nel dizionario: \n");
	while(temp1!=NULL)
	{
		cont_parole = cont_parole + 1;
		printf("\t [%d] -  %s \n ",cont_parole,temp1->parola);
		temp1 = temp1 ->next;
	}
	cont_parole = 0;
	printf("\t ------------------------------------\n");
	printf("\n");
	return testa_parola;
}
struct nodo_sinonimi* inserisci_sinonimo(struct nodo_sinonimi* testa_sinonimi) 
{
	int numero_sinonimo;
	char buffer_sinonimo[1024];
	int count_index = 0;
	printf("\n");
	printf("Digita un numero. Di quale parola vuoi aggiungere il sinonimo? \n");
	printf(">>");
	scanf("%d",&numero_sinonimo);
	struct nodo_parola* app = testa_parola;
	struct nodo_sinonimi* appoggio = testa_sinonimi;
	while(app!=NULL)
	{
		cont_parole = cont_parole + 1;
		if(cont_parole == numero_sinonimo)
		{
			printf("\t  - Parola %s selezionata - \n", app->parola);
			puts("\t Inserisci il sinonimo di questa parola:");
			fflush(stdin);
			if(fgets(buffer_sinonimo, 1024, stdin))
				buffer_sinonimo[strcspn(buffer_sinonimo, "\n")] = 0;
			strncpy(appoggio->sinonimo[appoggio->indice], buffer_sinonimo, 1024);
			appoggio->indice = appoggio->indice + 1;
		}
		app = app->next;
		appoggio = appoggio->next2;
	}
	cont_parole = 0;
	struct nodo_sinonimi* temp3 = testa_sinonimi;
	struct nodo_parola* temp2 = testa_parola;
	printf("\n");
	printf("\t Visualizzazione Vocabolario. \n");
	while(temp2!=NULL)
	{
		printf("\n");
		cont_parole = cont_parole + 1;
		printf("\t  Parola[%d]  = %s .\n",cont_parole,temp2->parola);
		printf("\n");
		for(int i=0;i<temp3->indice;i++) printf("\t    Sinonimo[%d.%d] = %s .\n",cont_parole,i,temp3->sinonimo[i]);
		temp2 = temp2 -> next;
		temp3 = temp3 -> next2;
	}
	printf("\n");
	cont_parole = 0;
	return testa_sinonimi;
}
struct nodo_parola* stampa_parola_che_iniziano_con_stessa_lettera(struct nodo_parola* testa_parola)
{
	char lettera;
	int counter = 0;
	bool match = false;
	if(testa_parola==NULL)
	{
		fprintf(stderr, "Il vocabolario è vuoto - Si prega di premere 1 per l'inserimento [*] - \n");
		printf("\n");
	}
	else
	{
		printf("Inserisci una lettera: ");
		fflush(stdin);
		scanf("%c", &lettera);
		struct nodo_parola* appoggio_parola = testa_parola;
		while(appoggio_parola!=NULL)
		{
			if(lettera == appoggio_parola->parola[0])
			{
				match = true;
				counter = counter + 1;
				printf(" \t %s \n", appoggio_parola->parola);
			}
			appoggio_parola = appoggio_parola->next;
		}
	}
	printf("\n");
	if(match)
		printf("\t Numero di parole nel dizionario che iniziano con %c  = [%d]. \n", lettera, counter);
	match = false;
	counter = 0;
	return testa_parola;
}
void stampa_sinonimi_di_una_parola()
{
	char parola[1024];
	struct nodo_sinonimi* temp3 = testa_sinonimi;
	struct nodo_parola* temp2 = testa_parola;
	if(testa_parola==NULL)
	{
		fprintf(stderr, "Il vocabolario è vuoto - Si prega di premere 1 per l'inserimento [*] - \n");
		printf("\n");
	}
	else
	{
		printf("Inserisci la parola di cui vuoi stampare i sinonimi: ");
		fflush(stdin);
		if(fgets(parola, 1024, stdin))
			parola[strcspn(parola, "\n")] = 0;
		while(temp2!=NULL)
		{
			if(strcmp(temp2->parola, parola)==0)
			{
				for(int i=0;i<temp3->indice;i++)
				{
					printf(" Sinonimo [%d] = %s \n", i+1, temp3->sinonimo[i]);
				}
			}
			temp2 = temp2->next;
			temp3 = temp3->next2;
		}
	}
}
