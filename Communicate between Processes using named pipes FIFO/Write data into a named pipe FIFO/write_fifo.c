#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char **argv)
{
	int file_descriptor;
	char my_string[42];
	
	mkfifo("FIFOpipe",0666); //0666 read write for owner group others
	puts("Enter the text to pass to the FIFO named pipe:");
	scanf("%s",my_string);
	
	file_descriptor=open("FIFOpipe",O_WRONLY);
	write(file_descriptor,my_string,sizeof(my_string));
	
	close(file_descriptor);
	
	
	return 0;
}
