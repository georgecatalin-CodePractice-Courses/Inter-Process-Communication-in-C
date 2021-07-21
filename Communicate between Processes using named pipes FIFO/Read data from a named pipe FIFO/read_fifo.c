#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#define BUFFERSIZE 42

int main(int argc, char **argv)
{
    int file_descriptor_read;
	char my_buffer[BUFFERSIZE];
	
	file_descriptor_read=open("FIFOpipe",O_RDONLY);
	read(file_descriptor_read,my_buffer,sizeof(my_buffer));
	printf("The text from the name pipe FIFO is: %s. \n",my_buffer);
	close(file_descriptor_read);
	return 0;
}
