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
Creare un programma che tenga traccia del numero di studenti che hanno una media maggiore di 18.
Ciascuno studente è identificato da un nome, cognome, matricola e una griglia di voti con la rispettiva media. 
CREARE LE SEGUENTI FUNZIONI:
-Inserimento
-Visualizzazione completa 
-Visualizzazione studenti solo con media >=18, dalla media più alta a quella più bassa
-Cancellazione di uno studente 
-Ordinamento alfabetico degli studenti 
*/

/*
P.P.S: Il programma è didattico. Esso è atto ad una preparazione sufficiente e discreta 
per la parte relativa alla programmazione in C in laboratorio e pertanto mi scuso per la banalità del programma.
*/
#define fflush(stdin) while ((getchar()) != '\n')
struct nodo
{
	char nome[1024];
	char cognome[1024];
	int matricola;
	char **griglia_voti;
	int numero_voti;
	double media;
	struct nodo* next;
};
struct nodo* testa = NULL;
struct nodo* app = NULL;

struct nodo* inserimento(struct nodo*);
struct nodo* visualizzazione(struct nodo*);
struct nodo* visualizzazione_maggiori_18(struct nodo*);
struct nodo* cancellazione(struct nodo*);
struct nodo* ordinamento_alfabetico(struct nodo*);

int main(int argc, char **argv)
{
	bool finito = false;
	int scelta;
	do{
		puts(" ");
		printf("1. Inserimento in lista [*] \n");
		printf("2. Visualizzazione completa lista [*] \n");
		printf("3. Visualizzazione studenti con media maggiore o uguale a 18 [*] \n");
		printf("4. Cancellazione di uno studente in base alla matricola [*] \n");
		printf("5. Ordinamento alfabetico degli studenti [*] \n");
		printf(">> ");
		scanf("%d", &scelta);
		switch(scelta)
		{
		case 1:
			{
				testa = inserimento(testa);
			}break;
		case 2:
			{
				testa = visualizzazione(testa);
			}break;
		case 3:
			{
				testa = visualizzazione_maggiori_18(testa);
			}break;
		case 4:
			{
				testa = cancellazione(testa);
			}break;
		case 5:
			{
				testa = ordinamento_alfabetico(testa);
			}break;
		default:
			{
				printf("Aborted [*] \n");
				finito = true;
			}break;
		}
	}while(finito!=true);
	return 0;
}
struct nodo* inserimento(struct nodo* testa)
{
	char buffer[1024];
	char nome[1024];
	char cognome[1024];
	int matricolaa;
	int voti_da_registrare;
	printf("Nome:");
	fflush(stdin);
	if(fgets(nome, 1024, stdin))
	{
		nome[strcspn(nome, "\n")] = 0;
	}
	printf("Cognome:");
	if(fgets(cognome, 1024, stdin))
	{
		cognome[strcspn(cognome, "\n")] = 0;
	}
	
	printf("Matricola:");
	scanf("%d", &matricolaa);
	
	fflush(stdin);

	printf("Numero dei voti da registrare: ");
	scanf("%d", &voti_da_registrare);
	char** array_double = (char**)malloc(sizeof(char*)*voti_da_registrare);
	
	fflush(stdin);
	for(int i=0;i<voti_da_registrare;i++)
	{
		printf("Voto [%d] :", i+1);
		array_double[i] = malloc(sizeof(char)*1024);
		if(fgets(buffer,1024,stdin))
		{
			buffer[strcspn(buffer,"\n")] = 0;
		}
		memcpy(array_double[i],buffer,1024);
	}
	
	struct nodo* nuovo_nodo = malloc(sizeof(struct nodo));
	strncpy(nuovo_nodo->nome,nome,1024);
	strncpy(nuovo_nodo->cognome,cognome,1024);
	nuovo_nodo->matricola = matricolaa;
	nuovo_nodo->numero_voti = voti_da_registrare;
	nuovo_nodo->griglia_voti = array_double;


	if(testa == NULL)
	{
		testa = nuovo_nodo;
		app = testa;
	}
	else
	{
		app->next = nuovo_nodo;
		app = nuovo_nodo;
	}
	return testa;

}
struct nodo* visualizzazione(struct nodo* testa)
{
	double media,somma=0;
	struct nodo* temp = testa;
	while(temp!=NULL)
	{
		puts(" ");
		printf("------------------------------------\n");
		printf("Nome : %s \n", temp->nome);
		printf("Cognome : %s \n", temp->cognome);
		printf("Matricola: %d \n", temp->matricola);
		printf("Numero voti : %d \n", temp->numero_voti);
		for(int i=0;i<temp->numero_voti;i++)
		{
			printf("Voto [%d] = %s \n", i+1, temp->griglia_voti[i]);
			somma += (double) strtol(temp->griglia_voti[i],NULL,10);
		}
		media = (double) somma / temp->numero_voti;
		temp->media = media;
		printf("Media dei voti: %f \n", temp->media);
		somma = 0;
		temp = temp->next;
	}
	return testa;
}
struct nodo* visualizzazione_maggiori_18(struct nodo* testa)
{
	struct nodo* app = testa;
	while(app!=NULL)
	{
		puts(" ");
		printf("------------------------------------\n");
		if((app->media)>=18.0)
		{
			printf("Nome : %s \n", app->nome);
			printf("Cognome : %s \n", app->cognome);
			printf("Matricola: %d \n", app->matricola);
     		printf("Media dei voti: %f \n", app->media);

		}
		app = app->next;
	}
	return testa;
}
struct nodo* cancellazione(struct nodo* testa)
{
	struct nodo* appoggio = NULL;
	struct nodo* prec = NULL;
	struct nodo* temp = testa;
	int matric;
	bool trovato = false;
	char nome[1024];
	printf("Inserisci la matricola dello studente da cancellare: ");
	scanf("%d",&matric);
	//se il nodo da cancellare è il primo 
	if(testa->matricola == matric)
	{
		appoggio = testa;
		testa = testa->next;
		free(appoggio); //cancello il primo nodo
		trovato = true;
	}
	else 	//altrimenti se il nodo da cancellare sta in mezzo
	{
		prec = temp;
		temp = temp->next;
		while((temp!=NULL)&&(trovato==false))
		{
			if(temp->matricola == matric)
			{
				prec->next = temp->next;
				free(temp);
				trovato = true;
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
struct nodo* ordinamento_alfabetico(struct nodo* testa)
{   /*
	L'ordinamento avviene per cognome
	*/
	struct nodo* temporaneamente = malloc(sizeof(struct nodo));
	struct nodo* appoggia = testa;
	struct nodo* i = appoggia;
	//appoggia = appoggia->next;
	/* Applichiamo un selection sort sulla lista */
	/*Due Indici.*/
	

	while(i!=NULL)
	{
		struct nodo* j = i->next;
		while(j!=NULL)
		{
			if(strcmp(i->cognome,j->cognome)>0)
			{
				printf("%s ---- %s \n", i->cognome,j->cognome);
				strncpy(temporaneamente->nome, i->nome, 1024);
				strncpy(temporaneamente->cognome, i->cognome, 1024);
				temporaneamente->matricola = i->matricola;
				temporaneamente->griglia_voti = i->griglia_voti;
				temporaneamente->numero_voti = i->numero_voti;
				temporaneamente->media = i->media;

				strncpy(i->nome, j->nome, 1024);
				strncpy(i->cognome, j->cognome, 1024);
				i->matricola = j->matricola;
				i->griglia_voti = j->griglia_voti;
				i->numero_voti = j->numero_voti;
				i->media = j->media;

				strncpy(j->nome, temporaneamente->nome, 1024);
				strncpy(j->cognome, temporaneamente->cognome, 1024);
				j->matricola = temporaneamente->matricola;
				j->griglia_voti = temporaneamente->griglia_voti;
				j->numero_voti = temporaneamente->numero_voti;
				j->media = temporaneamente->media;
			}

			j = j->next;
		}
		i = i->next;
	}
	return testa;
}