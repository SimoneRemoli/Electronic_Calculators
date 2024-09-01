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
Scrivere un convertitore che dato un numero decimale converta il numero in binario con un massimo di 8 bit.
Dopodichè il numero binario viene convertito in esadecimale.
*/

char* to_bin(char*, int);
void to_hex(char*,char*);
void quartettamelo(char**,char*);
int to_decimal(char*);



int main(int argc, char** argv)
{
	char numero_binario[1024]={0},numero_hex[1024]={0};
	int numero_decimale;
	char* nb;
	printf("Inserisci numero decimale: ");
	scanf("%d",&numero_decimale);
	nb = to_bin(numero_binario,numero_decimale);
	strncpy(numero_binario,nb,1024);
	printf("Il numero binario è = %s \n",numero_binario);
	to_hex(numero_binario,numero_hex);


	return 0;
}
int to_decimal(char* bin)
{
	char* app = (char*)malloc(sizeof(char)*1024);
	int counter = 1,somma_finale = 0,pos = 0;
	for(int i=strlen(bin)-1,j=0;i>=0;i--,j++)
	{
		*(app + j) = *(bin + i);
	}
	for(int i=0;i<strlen(app);i++)
	{
		if(*(app + i)=='1')
		{
			pos = i;
			while(pos--)
			{
				counter *=2;
			}
			somma_finale += counter;
			counter = 1;
		}

	}
	return somma_finale;

}
void to_hex(char* numero_binario, char* numero_hex)
{
	int indicex = 1;
	char** quartetto = (char**)malloc(sizeof(char*)*2);
	char* arraychar = (char*)malloc(sizeof(char)*6);
	char buffer[1024];
	quartettamelo(quartetto,numero_binario);
	int valore_decimale = 0;
	printf("Valore del quartetto[0] = %s \n", quartetto[0]);
	printf("Valore del quartetto[1] = %s \n", quartetto[1]);
	char car = 'A';
	int valore = (int)car;
	strcat(numero_hex,"0x");
	for(int i=0;i<6;i++)	*(arraychar + i) = (char)valore++;
	//printf("Valore di char = %s \n", arraychar);
	for(int i=0;i<2;i++)
	{

		valore_decimale = to_decimal(quartetto[i]);
		if(valore_decimale<10)
		{
			indicex = indicex + 1;
			sprintf(buffer,"%d",valore_decimale);
			strcat(numero_hex,buffer);
		}
		if(valore_decimale>=10)
		{
			for(int j=0;j<6;j++)
			{
				if(valore_decimale-10 == j)
				{
					indicex = indicex + 1;
					*(numero_hex + indicex) = *(arraychar + j);
				}
			}
		}
	}

	printf("Numero hex = %s \n", numero_hex);
	

}


void quartettamelo(char** quartetto, char* numero_binario)
{
	int counter = 0,indice = 0;
	char* app = malloc(sizeof(char)*1024);
	for(int i=0,j=0;i<strlen(numero_binario);i++,j++)
	{
		*(app + j) = *(numero_binario + i);
		counter = counter + 1;
		if(counter==4)
		{
			quartetto[indice] = malloc(sizeof(char)*1024);
			memcpy(quartetto[indice],app,1024);
			counter = 0;
			j=-1;
			indice = indice + 1;
		}
	}
}



char* to_bin(char* numero_binario, int numero_decimale)
{
	char* appoggio = (char*)malloc(sizeof(char)*1024);

	while(numero_decimale!=0)
	{
		if(numero_decimale%2==0)
			strcat(appoggio,"0");
		if(numero_decimale%2!=0)
			strcat(appoggio,"1");
		numero_decimale = numero_decimale / 2;
	}

	for(int j=0,i=strlen(appoggio)-1;i>=0;i--,j++) *(numero_binario+j) = *(appoggio + i);
	
	int lunghezza_numero = strlen(numero_binario);
	int numero_zeri = 8 - lunghezza_numero;
	memset(appoggio,0,1024);
	if(lunghezza_numero!=8)
	{
		for(int i=0;i<strlen(numero_binario);i++) *(appoggio + i) = *(numero_binario + i);
		memset(numero_binario,0,1024);
		for(int i=0;i<numero_zeri;i++)
		{
			*(numero_binario + i) = '0';
		}
		for(int i=numero_zeri,j=0;i<strlen(appoggio)+numero_zeri;i++,j++)
		{
			*(numero_binario + i) = *(appoggio + j);
		}
	}
	//printf("Valore di numero_binario = %s \n", numero_binario);
	return numero_binario;
}