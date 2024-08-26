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

/*Creare un programma che generi un numero randomico binario positivo di massimo 8 bit e che converta questo nell'equivalente 
rappresentazione esadecimale.
Dopodichè se al numero binario corrisponde un carattere ASCII (lettera) minuscolo convertirlo in maiuscolo e mostrare 
il risultato*/
int check(int);
void tobin(char*,int);
char* to_hex(char*);
void quartetto(char**,char*,int,int,int);
int to_decimal(char*);

int main(int argc, char** argv)
{
	srand((unsigned int)time(NULL));
	int numero_binario = 0;
	char binary[1024] = {0};
	char numero_esadecimale[1024] = {0};
	numero_binario = rand()%256;
	int ret = check(numero_binario);
	if(ret == -1)
	{
		printf("Il numero è negativo [*] \n");
		printf("Aborted [*] \n");
		return 0;
	}
	if(ret == -2)
	{
		printf("Il numero eccede 8 bit [*] \n");
		printf("Aborted [*] \n");
		return 0;
	}
	tobin(binary,numero_binario);
	printf("Numero decimale generato = %d e il suo valore binario = %s \n",numero_binario,binary);
	to_hex(binary);
	char carattere;
	int num_minuscolo;

	if((numero_binario>=65)&&(numero_binario<=90))
	{
		carattere = (char)numero_binario;
		printf("Corrispondente alla lettera = %c \n", carattere);
		num_minuscolo = numero_binario + 32;
		carattere = (char)num_minuscolo;
		printf("Corrispondente lettera minuscola = %c \n", carattere);
	}





	return 0;
}
char* to_hex(char *numero_bin)
{
	char *numero_hex = malloc(sizeof(char)*1024);
	char *app = malloc(sizeof(char)*1024);
	int lunghezza_numero = strlen(numero_bin);
	char* lettere = malloc(sizeof(char)*6);
	int numero_zeri = 8 - lunghezza_numero;

	if(numero_zeri>0)
	{
		for(int i=0;i<strlen(numero_bin);i++)
		{
			*(app + i) = *(numero_bin + i);
		}
		printf("app = %s \n", app);
		memset(numero_bin,0,1024);
		for(int i=0;i<numero_zeri;i++) *(numero_bin + i) = '0';
		for(int i=numero_zeri,j=0;j<strlen(app);i++,j++)
			*(numero_bin + i) = *(app+j);
	}
	printf("Numero binario esteso a 8 bit = %s \n", numero_bin);
	char** quartettino = (char**)malloc(sizeof(char*)*2);


	int val = 65,num_decimale=0;
	char buffered[1024];
	for(int i=0;i<6;i++) *(lettere + i) = (char)val++; //ABCDEF in lettere
	quartetto(quartettino,numero_bin,0,4,0); //il primo(0) quartetto va da 0 a 3 
	quartetto(quartettino,numero_bin,4,8,1); //il secondo(1) quartetto va da 4 a 7
	printf("Valore del primo quartetto = %s \n", quartettino[0]);
	printf("Valore del secondo quartetto = %s \n", quartettino[1]);
	strcat(numero_hex,"0x");
	for(int i=2;i<4;i++)
	{
		num_decimale = to_decimal(quartettino[i-2]);
		printf("Numero decimale = %d \n", num_decimale);
		if(num_decimale<=9)
		{
			sprintf(buffered,"%d",num_decimale); //CONVERT INT TO STRING
			strcat(numero_hex,buffered);
		}
		if(num_decimale>9)
		{
			for(int value=10;value<16;value++)
			{
				if(num_decimale == value)
				{
					*(numero_hex + i) = *(lettere + (value-10));
				}
			}
		}
		num_decimale = 0;



	}
	printf("Numero hex = %s \n", numero_hex);










}
int to_decimal(char* numbin)
{
	puts("-------------");
	int contatore = 1,somma=0,pos;
	char *invert = malloc(sizeof(char)*1024);
	for(int i=strlen(numbin)-1,j=0;i>=0;i--,j++) *(invert + j) = *(numbin + i);

	for(int i=0;i<strlen(invert);i++)
	{
		contatore = 1;
		if(*(invert + i) == '1')
		{
			pos = i;
			while(pos--)
			{
				contatore *=2;

			}
			somma +=contatore;
		}
	}
	return somma;

}
void quartetto(char** quartettino, char* numero_bin,int inizio, int fine,int numquart)
{
	quartettino[numquart] = malloc(sizeof(char)*1024);
	char *app = malloc(sizeof(char)*1024);

	for(int i=inizio,j=0;i<fine;i++,j++)	*(app + j) = *(numero_bin + i);
	memcpy(quartettino[numquart],app,1024);
	memset(app,0,1024);
	free(app);

}
int check(int num)
{
	if(num<0)
		return -1;
	if(num>255)
		return -2;
	return 1;
}
void tobin(char *numero_bin, int num)
{
	char* app = malloc(sizeof(char)*1024);
	while(num!=0)
	{
		if(num%2==0)
			strcat(app,"0");
		if(num%2==1)
			strcat(app,"1");
		num = num / 2;
	}
	for(int i=strlen(app)-1,j=0;i>=0;i--,j++)
		*(numero_bin + j ) = *(app + i);

}