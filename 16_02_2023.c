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
Scrivere un programma in C che accetti da tastiera un numero intero di tipo signed long . Il programma verifica se il
numero inserito è positivo o negativo.
Inoltre, il programma effettua dei controlli sulle rappresentazioni numeriche utilizzate dalla macchina in uso. In
particolare, esso verifica se, l'architettura su cui è in esecuzione, utilizza o meno la rappresentazione in
complemento a due per rappresentare i numeri negativi.
Inoltre, il programma verifica anche se la rappresentazione in memoria dei dati è in formato big endian o little
endian.
Al termine dell'esecuzione, il programma stampa le informazioni ottenute e le rappresentazioni del numero in input
nei formati:

-intero decimale;
-esadecimale;
-stringa binaria.
*/
signed long value;
int array[64];
bool check_positivo(signed long);
int function(void);
int complemento_a_due(void);
void big_or_little_endianess(void);
void to_binary(void);
int check_position_one_to_casting_complement_two_s(int *);
void worker(void*, signed long);

int main(int argc, char **argv)
{
	char buffer[1024];
	printf(">>");
	if(!fgets(buffer, 1024, stdin))
		fprintf(stderr, "Errore nell'acquisizione del numero [*] \n");
	value = strtol(buffer, NULL, 10);
	if(check_positivo(value))
	{
		printf("Il valore [%ld] è positivo [*] \n", value);
	}
	else
	{
		printf("Il valore [%ld] è negativo [*] \n", value); //se è negativo bisogna vedere la sua rappresentazione in C.A.2
	}
	printf("La rappresentazione %s il Complemento a Due [*] ! \n",complemento_a_due() ? "utilizza":"non utilizza");
	big_or_little_endianess();
	printf("\n");
	printf("Codifica del valore inserito:\n");
	printf("Decimale = %ld \n", value);
	to_binary();
	printf("Esadecimale: %#lx\n", value);
	return 0;
}
bool check_positivo(signed long value)
{
	if(value>0)
		return true;
	else
		return false;
}
int complemento_a_due(void)
{
	return (int)UINT_MAX == -1;
	/* Il numero più grande NON segnato è una parola di tutti 1 = -1 in C.A.2, basta verificare questa condizione.
	Se tale condizione è verificata allora il sistema adotta il Complemento a 2 (normalmente i PC moderni implementano tale approccio)
	su 4 bit: 1111 in C.A.2  = (-1)
	su 4 bit: 0001 = (1)*/
}
void big_or_little_endianess(void)
{
	/*This function shows a very simple program to detect the endianness of the system.*/
	int i = 1; //4 byte = 00000000000000000000000000000001 = 0x00000001
    if (*((char *)&i) == 1) 
    	puts("La macchina usa la rappresentazione LITTLE ENDIAN. [*]"); //0x01000000 = questo sarebbe il byte di valore 1 se fosse LITTLE ENDIAN (01) = 00000001
    else 
    	puts("La macchina usa la rappresentazione BIG ENDIAN."); //0x00000001
    return;
    /*è possibile scrivere in memoria il valore 1 utilizzando un tipo più grande di un byte. Se il valore è memorizzato in formato little endian,
	all'indirizzo di quella variabile si osserverà un byte di valore 1.*/
}
void to_binary(void) //64-bit
{
	signed long modulo;
	int *ptr = malloc(sizeof(int)*64);
	int i = 63;
	int *pointer_position = malloc(sizeof(int)*1);
	if(check_positivo(value)==false)
	{
		modulo = llabs(value);
		printf("Il numero è negativo, questo è il suo valore assoluto = %ld \n", modulo);
		worker(ptr, modulo);
		*pointer_position = check_position_one_to_casting_complement_two_s(ptr);
		for(int i=*(pointer_position) - 1; i>=0; i--) 
			if(*(ptr + i) == 0)
				*(ptr + i) = 1;
			else
				*(ptr + i) = 0;
		fprintf(stdout,"Questa è la codifica binaria del valore in Complemento a Due (%ld): ", value);
		for(int i=0;i<64;i++) printf("%d", *(ptr + i)); printf("\n");
	}
	else
		worker(ptr, value);
}
int check_position_one_to_casting_complement_two_s(int *ptr)
{
	for(int i=63;i>=0;)
	{
		if(*(ptr + i) == 1)
			return i;
		i--;
	}
}
void worker(void*arg, signed long value)
{
	int *ptr1 = (int*)arg; //Siamo aperti a tutto ;)
	int i = 63;
	while(value != 0)
	{
		if(value %2 == 0)
			*(ptr1 + i) = 0;
		else
			*(ptr1 + i) = 1;
		value = value / 2;
		i = i - 1;
	}
	fprintf(stdout,"Questa è la codifica binaria del valore assoluto: ");
		for(int i=0;i<64;i++) printf("%d", *(ptr1 + i)); printf("\n");
}