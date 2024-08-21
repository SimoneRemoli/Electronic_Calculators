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
Implementare un programma che prenda come input una frase composta da parole, ad esempio:
>>ciao mamma sono una brava persona che ama tanto gli Onigiri
In totale questa frase ha 11 parole.
Questi undici termini vengono inseriti all'interno di un vettore che li memorizza in ordine di inserimento e in 
un altro vettore che li memorizza in reverse.
Se l'elemento i-esimo del vettore normale e del vettore reverse coincidono alla stringa iniziale devo sostituire il
carattere '*' a tutte le lettere che sono contenute nell'uguaglianza.
*/
int contatore = 0;
long* acquisizione(char*,char**);
void analize(long*,char*,int);
void views_vettore(long*,int);
long* reverse(long*);
void check(long*,long*,char**);
void scrivi_in_char(char**,char*,int);
char buffer[1024];
char copy_buffer[1024];

int main(int argc, char **argv)
{
	long *ptr_view,*ptr_reverse;
	char **ptr_char = (char**)malloc(sizeof(char*)*1024);
	ptr_view = acquisizione(buffer,ptr_char);
	for(int i=0;i<contatore;i++) printf("[%d] parola = %s \n",i+1,*(ptr_char + i));
	views_vettore(ptr_view,contatore);
	ptr_reverse = reverse(ptr_view);
	views_vettore(ptr_reverse,contatore);
	check(ptr_view,ptr_reverse,ptr_char);








	return 0;
}
long* reverse(long* view)
{
	long* reverse = (long*)malloc(sizeof(long)*1024);
	for(int i=0,j=contatore-1;i<contatore;i++,j--)
	{
		*(reverse + i) = *(view + j);
	}
	return reverse;
}
long* acquisizione(char* buffer,char **charptr)
{
	long* ptr = malloc(sizeof(long)*1024);
	int num = 0;

	if(fgets(buffer,1024,stdin))
		buffer[strcspn(buffer,"\n")] = 0;

	strcpy(copy_buffer,buffer);

	char* token =  strtok(buffer," ");
	while(token!=NULL)
	{
		//printf(" %s \n", token); // man mano le parole spezzettate
		analize(ptr,token,contatore);
		scrivi_in_char(charptr,token,contatore);
		token = strtok(NULL, " ");
		contatore = contatore + 1;
	}
	printf("Numero di parole nella frase = %d \n", contatore);
	return ptr;


	/*ora verrà creato un array di N elementi con la somma dei valori ASCII*/



}
void scrivi_in_char(char** d, char* token, int contatore)
{
	d[contatore] = malloc(sizeof(char)*1024);
	memcpy(d[contatore], token, 1024);
}
void analize(long* ptr, char*token, int contatore)
{
	long num = 0;
	for(int i=0;i<strlen(token);i++)
	{
		num = 0;
		num = (long)token[i]; //ascii num
		*(ptr + contatore) += num;
	}
}
void views_vettore(long* ptr, int count)
{
	puts(" ");
	for(int i=0;i<count;i++)
	{
		printf("Valore in Ascii della [%d] parola = %ld \n", i+1, *(ptr + i));
	}
	puts(" ");
}
void check(long* ptr_dritto, long* ptr_inverso, char** d)
{
	bool trovato = false;
	int indexer = 0;
	char *appoggio;
	int jj = 0,ii=0;
	for(int i=0;i<contatore;i++)
	{
		if(*(ptr_dritto + i)==(*(ptr_inverso + i)))
		{
			trovato = true;
			appoggio = *(d+i);
			printf("Valore della stringa uguale = %s \n", appoggio);
			printf("Valore di strlen appoggio = %ld \n", strlen(appoggio));
			
			while(jj<strlen(appoggio))
			{
				if(copy_buffer[ii] == appoggio[jj])
				{
					printf("Valore di buffer = %c  e indice = %d\n", copy_buffer[ii],ii);
					*(copy_buffer + ii ) = '*';
				}
				if(ii==strlen(copy_buffer)-1)
				{
					ii=0;
					jj = jj + 1;
				}
				ii = ii + 1;
			}

			ii=0;
			jj=0;
		}
	}
	printf("Visualizzazione della stringa iniziale = %s \n", copy_buffer);
}