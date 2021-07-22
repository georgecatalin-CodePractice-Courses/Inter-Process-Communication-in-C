#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdint.h> //to use uintptr_t

static void signal_handler_function(int sig);

int main(int argc, char *argv[])
{
	int return_value;
	return_value=(uintptr_t) signal(SIGINT,signal_handler_function);
	
	if (return_value!=0)
	{
		perror("Signal:");
		puts("Unable to raise the signal.");
		exit(0);
	}
	
	puts("Now I am going to a raise a signal by myself using the raise() function...");
	
	return_value=raise(SIGINT); //No need for CTRL+C as it is raised independently of that
	 
	if(return_value!=0)
	{
		perror("Raise():");
		puts("Unable to raise a signal by myself with raise()...");
		exit(0);
	}
	
	puts("Exiting program now...");
	return 0;
}
	

static void signal_handler_function(int sig)
{
	puts("I caught the signal....");
}
