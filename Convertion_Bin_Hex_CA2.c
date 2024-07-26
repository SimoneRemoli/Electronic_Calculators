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
----------------------------------------------------------------------
You have to take a decimal number as input, convert it to binary and,
only after having performed this first conversion, convert the binary
to hexadecimal and make it the 2's complement;
----------------------------------------------------------------------
				Simone Remoli    :)
----------------------------------------------------------------------
*/
int max_range = 255;
void from_decimal_to_bin(int, char*);
void from_bin_to_hexadecimal(char*, char*, char**);
int from_bin_to_decimal(char*);
void complement_two(char*,char*);
void complement_two_recursive(char*, char*,int);
bool fine = false;
int main(int argc, char **argv)
{
	int numero_decimale;
	char binary[1024]={0};
	char hexadecimal[1024] = {0};
	char complemento_a_due[1024] = {0};
	char **quartetto = malloc(sizeof(char*)*2);
	quartetto[0] = malloc(sizeof(char)*1024);
	quartetto[1] = malloc(sizeof(char)*1024);

	printf("Insert decimal number : ");
	scanf("%d", &numero_decimale);
	if(numero_decimale>max_range)
	{
		perror("Aborted - maximum 8-bits range [*] \n");
		return 0;
	}
	puts(" ");
	printf("\t You insert decimal number  = %d \n", numero_decimale);
	from_decimal_to_bin(numero_decimale, binary); 
	from_bin_to_hexadecimal(binary,hexadecimal,quartetto);
	complement_two(binary, complemento_a_due);
	int indice_ricorsivo = strlen(binary)-1;
	complement_two_recursive(binary,complemento_a_due,indice_ricorsivo);
	printf("\t Valore del Complemento a Due (approccio ricorsivo) = %s \n",complemento_a_due);
	free(quartetto[0]);
	free(quartetto[1]);
	return 0;
}
void complement_two_recursive(char* binary, char* complemento_a_due, int indice_ricorsivo)
{
	if(fine)
	{
		if(*(binary + indice_ricorsivo)=='1')
			*(complemento_a_due + indice_ricorsivo ) = '0';
		else
			*(complemento_a_due + indice_ricorsivo ) = '1';
	}
	if(indice_ricorsivo==0)
		return;
	if(fine==false)
	{
		if(*(binary + indice_ricorsivo)=='0')
			*(complemento_a_due + indice_ricorsivo ) = *(binary + indice_ricorsivo);
		if(*(binary + indice_ricorsivo)=='1')
		{
			fine = true;
			*(complemento_a_due + indice_ricorsivo ) = *(binary + indice_ricorsivo);
		}
	}
	complement_two_recursive(binary,complemento_a_due,indice_ricorsivo-1);
}
void complement_two(char* binary, char* complemento_a_due)
{
	bool check = false;
	for(int i=strlen(binary)-1;i>=0;i--)
	{
		if(check == false)
		{
			if(*(binary + i)=='0')
				*(complemento_a_due + i ) = *(binary + i);
			if(*(binary + i) =='1')
			{
				check = true;
				*(complemento_a_due + i ) = *(binary + i);
				i--;
			}
		}
		if(check)
		{
			if(*(binary + i)=='1')
				*(complemento_a_due + i ) = '0';
			else
				*(complemento_a_due + i ) = '1';
		}

	}
	printf("\t Valore del Complemento a Due (approccio iterativo) = %s \n",complemento_a_due);
}
void from_bin_to_hexadecimal(char* binary, char* hexadecimal, char** quartetto)
{
	int indexer = 0,val_base = 10;
	char value;
	int res[2]={0};
	int laster = 1;
	while(laster<=2)
	{
		for(int i=indexer*4,j=0;i<laster*4;i++,j++)
		{
			quartetto[indexer][j] = *(binary + i);
		}
		indexer = indexer + 1;
		laster = laster + 1;
	}
	/*char alfa = 'A';
	int alfaint = (int) alfa;
	alfaint++;
	char beta = (char)alfaint;
	printf("Valore del carattere beta = %c \n", beta); // B
	*/
	char alfa = 'A';
	int alfaint = (int)alfa;
	char *caratterini = malloc(sizeof(char*)*10);
	int contatore_car = 0;
	while(contatore_car!=10)
	{
		*(caratterini+contatore_car) = (char)alfaint++;
		contatore_car++;
	}
	/*for(int i=0;i<6;i++)
	{
		printf("Valore carattere[%d] = %c \n", i, caratterini[i]);
	}*/
	/*while(contatore_car--)
	{
		printf("Valore di caratterini = %c \n", *caratterini++);
		se è già popolato questo modus operandi è 'funzionante'. Smile ;)
	}
	*/
	/*printf("Valore di quartetto[0] = %s \n", quartetto[0]);
	printf("Valore di quartetto[1] = %s \n", quartetto[1]);
	*/
	res[0] = from_bin_to_decimal(quartetto[0]);
	res[1] = from_bin_to_decimal(quartetto[1]);
	strcat(hexadecimal, "0x");
	for(int i=0; i<2; i++)
	{
		value = res[i] + '0';
		val_base = 10;
		if(res[i]>9)
		{
			for(int j=0;j<6;j++,val_base++)
			{
				if(res[i]==val_base)
					*(hexadecimal + (i+2)) = *(caratterini + j);
			}
		}
		else
		{
			*(hexadecimal + (i+2)) = value;
		}
	}
	printf("\t Hexadecimal Convertion = %s \n", hexadecimal);
}
int from_bin_to_decimal(char *val)
{
	int power = 1,num,res=0;
	int app;
	for(int i=strlen(val)-1;i>=0;i--)
	{
		num = val[i] - '0';
		app = strlen(val)-(i+1);
		while(app--)
		{
			power *= 2;
		}
		res += num*power;
		power = 1;
	}
//	printf("Valore del res = %d \n", res);
	return res;
}
void from_decimal_to_bin(int numero_decimale, char* binary)
{
	int max = 8;
	while(numero_decimale!=0)
	{
		if(numero_decimale%2==0)
			strcat(binary, "0");
		else
			strcat(binary, "1");
		numero_decimale = numero_decimale / 2;
	}
	char* temp = malloc(sizeof(char)*1024);
	char* temp2 = malloc(sizeof(char)*1024);
	char *app = temp;
	char *app2 = temp2;
	for(int i=strlen(binary)-1,j=0;i>=0;i--,j++) temp[j] = *(binary+i);
	int val = strlen(binary), i=0, maximum_value = 0;
	maximum_value = val;
	//printf("Valore della lunghezza della stringa = %d \n", val);
	while(val--)
	{
		*(binary + i) = *app++;
		i = i + 1;
	}
	free(temp);
	int num_zeri = max-maximum_value;
	if(maximum_value<8) //il procedimento va effettuato se il numero delle cifre binarie non sono sufficienti per coprire l'ottetto
	{
		if(maximum_value<max)
		{
			for(int i=0;i<strlen(binary);i++)
			{
				*(temp2 + num_zeri) = binary[i];
				num_zeri = num_zeri + 1;
			}
		}
		for(int i=0;i<(max-maximum_value);i++) *(app2 + i) = '0';
		i=0;
		while(max--)
		{
			*(binary + i ) = *app2++;
			i = i + 1;
		}
	}
	free(temp2);
	printf("\t Binary Convertion = %s \n", binary);
	return;

}
