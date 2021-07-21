/* ********************************************
 * I will use a fork() function to create a child process out of a parent process.
 * Then, I shall write into the pipe using the child process newly created and read from the pipe in the parent process.
 * **************************
 * In the above manner I shall simulate the communication between two different processes
 * ******************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


#define max 50

int main(int argc, char **argv)
{
	char write_string[max];
	char read_buffer[max];
	
	int array_pipe[2];
	pid_t my_process;
	
	if(pipe(array_pipe)!=0)
	{
		perror("pipe:");
		exit(1);
	}
	
	my_process=fork();
	
	if (my_process>=0) //either in the child process or in the parent process
	{
		if(my_process==0)
		{
			//I am in the child process now
			puts("Enter the message you wish to pass via pipe:");
			gets(write_string);
			write(array_pipe[1],write_string,strlen(write_string)); //it writes so has to includ the last \0 character from the string in strlen 
			exit(0);
		}
		else 
		{
			read(array_pipe[0],read_buffer,sizeof(read_buffer));
			printf("The message read from the pipe is: %s . \n",read_buffer);
			exit(0);
		}
	}
	else
	{
		perror("fork:");
		exit(2);
	}
		
	
	return 0;
}
