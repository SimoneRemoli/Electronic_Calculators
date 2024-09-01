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


//STRTOK: 192.168.1.2/24

/*192
168
1
2
24
*/

int main(int argc, char**argv)
{

	char buffer[1024];
	char* token;
	if(fgets(buffer,1024,stdin))
		buffer[strcspn(buffer,"\n")] = 0;
	token = strtok(buffer,".");
	while(token!=NULL)
	{
		for(int i=0;i<strlen(token);i++)
		{
			if(*(token + i)=='/')
			{
				token = strtok(token,"/");
			}
		}
		printf("Token = %s \n", token);
		token = strtok(NULL, ".");
	}
	return 0;
}