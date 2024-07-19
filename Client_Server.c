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

/*Simuliamo un invio di pacchetti tra client e server.
Il client invia una richiesta al proxy, la richiesta rappresenta la visualizzazione di una specifica pagina.
Qualora il proxy avesse a disposizione la pagina richiesta dal client deve fornire la pagina con un codice di status 202.
Se il proxy non ha a disposizione la pagina richiesta deve interrogare un server secondario per richiedere la pagina e 
restituirla al client. Va da sé che ogni volta che il proxy contatta il server (oppure anche se non lo contatta), deve salvare
in una propria lista tutti i nomi dei domini richiesti.
Il proxy mantiene in una propria lista fino ad un massimo di 5 siti web.
Il programma è stato volutamente scritto in maniera semplice, essendo un programma a scopo puramente didattico.
*/

#define ACK() (printf("ACK - Server. Pagina ottenuta.\n"))
#define fflush(stdin) while ((getchar()) != '\n')

struct nodo
{
	char buffer[1024];
	struct nodo* next;
};

struct nodo* temp = NULL;
struct nodo* testa = NULL;

struct server 
{
	char* nome_server;
};

struct nodo* richiesta_al_primo_server(struct nodo*);
char* richiesta_principale(void*, void*);
struct nodo* views(struct nodo*);
long num_richieste=0;
struct nodo* visualizza_lista(struct nodo*);
int scelta;

int main(int argc, char **argv)
{
	bool fine = false;
	struct server* first = malloc(sizeof(struct server));
	first->nome_server = "Server_Near";
	char *str1 = "Benvenuto/a.";
	char *str2;
	char str3[1024];
	strncpy(str3,str1,1024); //o memcpy uguale
	//memcpy(str3,str1,1024);
	//printf("Valore di str2 = %s \n valore di str3 = %s \n", str2,str3);
	printf("Ciao e %s \n", str3);
	//richiesta_al_primo_server(proxy);
	printf("Se vuoi richiedere un sito la richiesta verrà girata ad un primo server '%s'. \n", first->nome_server);
	puts(" ");
	do{
		puts(" ");
		printf("1. Richiedi pagina [*] \n");
		printf("2. Visualizza lista server [*] \n");
		printf("9. Chiudi [*] \n");
		printf(">>");
		scanf("%d", &scelta);
		switch(scelta)
		{
		case 1:
			{
				testa = richiesta_al_primo_server(testa);

			}break;
		case 2:
			{
				testa = visualizza_lista(testa);
			}break;
		default:
			{
				printf("Aborted [*] \n");
				fine = true;
			}break;
		}
	}while(fine != true);
	return 0;
}

struct nodo* richiesta_al_primo_server(struct nodo* testa)
{
	bool is_present = false;
	struct nodo* appoggio = testa;
	char richiesta[1024];
	char *risposta;
	puts(" ");
	puts("Richiesta Server. ");
	puts("Inserisci il nome del sito da richiedere. ");
	printf(">>");
	fflush(stdin);
	if(fgets(richiesta, 1024, stdin))
	{
		richiesta[strcspn(richiesta, "\n")] = 0;
	}
	num_richieste = num_richieste + 1;
	if(num_richieste==6)
	{
		testa = NULL;
		num_richieste = 1;
	}
	struct nodo* nuovo_nodo = malloc(sizeof(struct nodo));
	if(testa == NULL)
	{
		printf("Il dominio digitato non è presente nel Proxy [*] - Richiesta al server centrale [*] \n");
		risposta = richiesta_principale((void*)num_richieste, richiesta);
		strncpy(nuovo_nodo->buffer, risposta,1024); // in first->nodo->buffer c'è il valore 
		printf("'%s' aggiunto nella lista Proxy [*]  \n", nuovo_nodo->buffer);
		testa=nuovo_nodo;
		temp=testa;
	}
	else
	{
		while(appoggio!=NULL)
		{
			if(strcmp(appoggio->buffer,richiesta)==0)
			{
				printf("Il dominio è già presente in lista [*] - Nessuna richiesta al server centrale [202] [*] \n");
				puts("Pagina ottenuta [*] ");
				is_present = true;
			}
			appoggio = appoggio->next;
		}
		if(is_present==false)
		{
			printf("Il dominio digitato non è presente nel Proxy [*] - Richiesta al server centrale [*] \n");
			risposta = richiesta_principale((void*)num_richieste, richiesta);
			strncpy(nuovo_nodo->buffer, risposta,1024); // in first->nodo->buffer c'è il valore 
			printf("'%s' aggiunto nella lista Proxy [*]  \n", nuovo_nodo->buffer);
			temp->next = nuovo_nodo;
			temp = nuovo_nodo;
		}
	}
	return testa;
}
char* richiesta_principale(void *req, void *str)
{
	long val = (long)req;
	char* stringa = (char*)str;
	puts(" ");
	sleep(3);
	printf("Richiesta numero ''%ld'' con pagina ''%s'' [*] - Presso Server Principale [*] \n", val, stringa);
	sleep(1);
	ACK();
	strcat(stringa,".com");
	return stringa;
}
struct nodo* visualizza_lista(struct nodo* testa)
{
	printf("\n");
	struct nodo* app = testa;
	long count = 1;
	if(testa==NULL)
		printf("La lista è vuota [] \n");
	while(app!=NULL)
	{
		printf(" [%ld] - [%s] \n", count, app->buffer);
		app = app->next;
		count = count + 1;
	}
	if(count == 6)
		testa = NULL;
	puts(" ");
	return testa;
}