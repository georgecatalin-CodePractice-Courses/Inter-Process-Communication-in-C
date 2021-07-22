#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static void signal_handler_function(int sig);

int main(int argc, char **argv)
{
	//register the signal handler routines before using the custom behaviour of the signals
	signal(SIGUSR1,signal_handler_function);
	scanf("\n"); //this keeps the process for closing
	
	return 0;
}

static void signal_handler_function(int sig)
{
	puts("The signal has been received.");
}
