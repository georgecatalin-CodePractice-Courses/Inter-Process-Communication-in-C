#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(int argc, char **argv)
{
	// I used ps -aux and found the process id of the running recipient_process executable is 14300
	kill(14300, SIGUSR1);
	scanf("\n"); //keeps the process from closing down
	return 0;
}
