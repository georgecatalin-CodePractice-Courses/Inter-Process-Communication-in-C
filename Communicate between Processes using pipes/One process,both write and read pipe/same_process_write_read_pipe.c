#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define max 42

int main(int argc, char **argv)
{
	char my_string[max];
	int  arrayPipe[2];
	
	if(pipe(arrayPipe)<0)
	{
		exit(1); //pipe() did not return 0 (0=Successful) , then it exits throwing value =1
	}
	
	puts("Enter the first message to write into the pipe:");
	gets(my_string);
	write(arrayPipe[1],my_string,max);
	
	puts("Enter the second message to write into the pipe:");
	gets(my_string);
	write(arrayPipe[1],my_string,max);
	
	puts("");
	puts("Read the messages from the pipe in FIFO Order:");
	read(arrayPipe[0],my_string,max);
	printf("First message : %s \n",my_string);
	
	read(arrayPipe[0],my_string,max);
	printf("The second message from the pipe: %s \n",my_string);
	
	
	return 0;
}
