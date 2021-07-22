#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

//these are signal handler routine
static  void signal_handler_for_ctrl(int sig);       //unlike global functions in C, access to static functions is restricted into the file where they are declared. Therefore, when we want to restrict access to functions, we make them static. Another reason for making functions static, is reusing the name of the functions in other files
static void signal_handler_for_abort(int sig);

int main(int argc, char *argv[])
{
	//Register the signal handler routines before using them to customize behaviour when receiving these signals
	signal(SIGINT,signal_handler_for_ctrl);
	signal(SIGABRT,signal_handler_for_abort);
	
	char myChar;
	puts("Abort program Y/N?");
	scanf("%c", &myChar);
	
	if(myChar=='Y' || myChar=='y')
	{
		abort();
	}
	
	return 0;
}


static void signal_handler_for_ctrl(int sig)
{
	puts("Hey boss... the CTRL+C was pressed.");
	exit(0);
}

static void signal_handler_for_abort(int sig)
{
	puts("Hey boss... this process is aborted.");
	exit(0);
}
