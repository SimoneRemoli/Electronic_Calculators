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

/*generare tutte le permutazioni di una parola.*/
/*Ecco tutte le combinazioni di lettere che compongono la parola "roma": (4!)-1
roma
ramo
roam
rmoa
rmao
orma
oram
omra
omar
oarm
oamr
mora
moar
mroa
mrao
maor
maro
arma
armo
aorm
aomr
amro
amor
*/

void swap(char *, char *);
void reverse(char *, int, int);
int next_permutation(char *, int);


int main() {
	char str[1024];
	printf("Insert word: ");
	if(fgets(str,1024,stdin))
		str[(strcspn(str,"\n"))] = 0;
    int len = strlen(str);
    
    // Ordina la stringa per garantire che la prima permutazione sia la più piccola lessicograficamente
    for (int i=0; i<len-1; i++) 
    {
        for (int j = i+1; j<len; j++)
        {
            if (str[i] > str[j])
            {
                swap(&str[i], &str[j]);
            }
        }
    }
    int cont = 1;
    do 
    {

        printf("%d. -> %s\n",cont,str);
        cont = cont + 1;

    }while (next_permutation(str, len));
    
    return 0;
}

void swap(char *x, char *y) 
{
    char temp = *x;
    *x = *y;
    *y = temp;
}

void reverse(char *str, int start, int end)
{
    while (start < end) 
    {
        swap(&str[start], &str[end]);
        start++;
        end--;
    }
}

int next_permutation(char *str, int len)
{
    int i = len - 2;
    while (i >= 0 && str[i] >= str[i+1])
        i--;

    if (i < 0)
        return 0;
    
    int j = len - 1;
    while (str[j] <= str[i])
        j--;
    
    swap(&str[i], &str[j]);
    reverse(str, i + 1, len - 1);
    return 1;
}

/*
Spiegazione del codice:

    Funzione swap: Scambia i valori di due caratteri.
    Funzione reverse: Inverte l'ordine dei caratteri in una stringa dal start all'end.
    Funzione next_permutation: Genera la prossima permutazione lessicografica della stringa:
        Trova il punto di inversione.
        Trova il valore giusto per lo scambio.
        Effettua lo scambio.
        Inverte la parte restante della stringa.
        Ritorna 1 se c'è una prossima permutazione, altrimenti 0.
        Trova il punto di inversione:
		Parte da destra e cerca il primo carattere che è inferiore al carattere successivo (str[i] < str[i + 1]).
    	Se non trova tale carattere (i.e., i < 0), significa che la stringa è l'ultima permutazione possibile 
    	(es. "romz" è la più grande permutazione lessicografica di "roma").
		Trova il valore giusto per lo scambio:
		Trova il più piccolo carattere a destra di i che è maggiore di str[i].
    	Scambia str[i] con questo carattere (str[j]).
    	Inversione: Inverte la sequenza di caratteri a destra di i per ottenere la prossima permutazione più piccola.
    	Ritorna: Ritorna 1 se è stata generata una nuova permutazione, 0 se non ce ne sono più.
    Funzione main:
        Inizializza la stringa e la sua lunghezza.
        Ordina la stringa per ottenere la prima permutazione lessicografica.
        Usa un ciclo do-while per stampare ogni permutazione fino a quando non ci sono più permutazioni disponibili.

Questo approccio iterativo utilizza la funzione next_permutation per generare ogni permutazione successiva in modo efficiente senza usare la ricorsione.
*/