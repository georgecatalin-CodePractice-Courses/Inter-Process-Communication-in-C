/*
 * tcp_server.c
 *
 * Steps necessary for a TCP Server to handle multiple client connections (multiplexing)
 * *************************************************************************************
 * 1. Initialize the necessary variables
 * 2. Create the master socket file descriptor
 * 3. Bind
 * 4. Listen
 * 5. Initialize and fill the "readfds" collection of file descriptors
 * 6. Select
 * 7. Accept the connection and create the communication file descriptor
 * 8. Service the requests from the client
 * 9. Close the connection
 * 10. Go back to step 5.
 * ***************************************************************************************
 *
 * Created on: Jun 2, 2021
 * Author: George Calin
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include <errno.h>
#include "common.h"
#include <arpa/inet.h>

#define NO_MAX_CLIENTS 32
#define SERVER_PORT 2021

int monitored_set_of_filedescriptors[33];  //32 possible clients with each own's communication file descriptor + master socket file descriptor
char data_buffer[1024];

test_struct_t test_struct; //declare the test_struct variable of type test_struct_t
result_struct_t result_struct; //declar the result variable of type result_struct_t


void run_tcp_server_communication(void);

/* *** Helper functions prototypes *** */
static void initialize_set_of_filedescriptors(); //static functions in C are functions that are visible only to other functions in the same file (opposite to global functions)
static void add_to_monitored_set_of_filescriptors(int socket_filescriptor);
static void remove_from_monitored_set_of_filescriptors(int socket_filescriptor);
static void re_initialize_collection_of_filedescriptor(fd_set *pointer_to_fd_set);
static int get_max_filedescriptor();


int main(int argc, char *argv[])
{
	run_tcp_server_communication();

	return 0;
}

void run_tcp_server_communication(void)
{
	/* *** Initialization *** */
	int master_socket_file_descriptor =0; //this is the master socket file descriptor which handles and listens for new connection requests
	int sent_received_bytes=0;
	int address_length=0;
	int opt=1;

	int communication_socket_file_descriptor=0; //this is the file descriptor variable which will be used for communication between server and client, after the initiation of the communication phase which is handled by the master socket file descriptor
	fd_set collection_file_descriptors; // Select() system call watches the file descriptors in this selection and it unlocks any time when data arrives and is meant for a file descriptor in there

	struct sockaddr_in server_address; //a special struct container to hold the data about the address of the server
	struct sockaddr_in client_address; //a special struct container to hold the data about the address of the client(s)

	/* *** remove all the file descriptors in the array where the server looks into *** */
	initialize_set_of_filedescriptors();

	/* *** Create Master socket file *** */
	master_socket_file_descriptor=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

	if(master_socket_file_descriptor==-1)
	{
		printf("Error when creating the master socket file descriptor.\n");
		perror("Master socket file descriptor:");
		exit(EXIT_FAILURE);
	}

	/* *** Specify the information about the Server to the Operating System *** */
    server_address.sin_family=AF_INET; //This refers to IPv4 network packages
    server_address.sin_port=SERVER_PORT; //The server will process only data arriving at this port
    server_address.sin_addr.s_addr=INADDR_ANY; //The Linux Operating System will send all the packages for this destination address of the server to the TCP server

    address_length=sizeof(struct sockaddr);

    /* *** Bind the server to incoming data, that is telling the Linux Kernel that any data received with the destination IP of this server and the port defined
     * will be forwarded to this TCP Server process.
     * Note: a server machine can run multiple processes to process different kind of data and render services to multiple clients.
     * ***/
    int result_of_bind_operation=bind(master_socket_file_descriptor,(struct sockaddr *) &server_address,address_length);

    if(result_of_bind_operation==-1)
    {
    	printf("Error when binding.\n");
    	perror("Socket bind failed:");
    	return;
    }


	/* *** Listen. Announce Linux Operating System to maintain the queue of incoming connection to the maximum number of possible clients *** */
    int result_of_listen_operation=listen(master_socket_file_descriptor,NO_MAX_CLIENTS);

    if(result_of_listen_operation==-1)//unsuccessful
    {
    	printf("Error while listening.\n");
    	perror("Listen failed:");
    	return;
    }


	/* *** Add the Master Socket file to the set of file descriptors which are being monitored *** */
    add_to_monitored_set_of_filescriptors(master_socket_file_descriptor);


    /* *** Start an infinite which is the way the server is functioning *** */
    while(1)
    {
    	/* *** Initialize and fill in the collection of file descriptors *** */
    	re_initialize_collection_of_filedescriptor(&collection_file_descriptors);//copy all the monitored file descriptors to the fd_set collection being watched

    	printf("The process is blocked on the select() system call now.\n");

    	select(get_max_filedescriptor()+1,&collection_file_descriptors, NULL, NULL, NULL);

    	/* *** at this point the execution of code stops, and the process is listening for new connections. It unlocks when new data arrives.
    	 * The server process needs to check to whom such data is addressed to :
    	 * is it addressed to the master socket file descriptor (it means it is a connection initiation request from a new client) OR
    	 * is it addressed to an existing communication file descriptor
    	 */

    	if(FD_ISSET(master_socket_file_descriptor,&collection_file_descriptors))
    	{
    		/* *** Master socket file descriptor is activated, hence a new client connects to the server right now *** */
    		printf("A new connection initiation request message has been received. Accepting the connection...\n");

    		/* *** at this point the server process invokes the accept() system call and this system call creates a new (communication) file descriptor associated with the new client
    		 * This new communication file descriptor will be used by the server process to communicate with that client during the entire life of that connection.
    		 */

    		communication_socket_file_descriptor=accept(master_socket_file_descriptor, (struct sockaddr *)&client_address,&address_length);

    		if(communication_socket_file_descriptor==-1)
    		{
    			/* *** handle errors in case constructing file descriptors *** */
    			printf("Accept() system call error...\n");
    			perror("Accept() error:");
    			exit(EXIT_FAILURE);
    		}

    		/* *** add the communication file descriptor to the array of file descriptors *** */
    		add_to_monitored_set_of_filescriptors(communication_socket_file_descriptor);

    		printf("A connection was accepted from the client %s at port %u.\n",inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));

    	}
    	else //in this case the new data is meant for an existing client, and not from a new client
    	{
    		communication_socket_file_descriptor=-1;

    		for(int i=0;i<NO_MAX_CLIENTS;i++)
    		{
    			if(FD_ISSET(monitored_set_of_filedescriptors[i],&collection_file_descriptors))
    			{
    				/* *** verify which is the existing client which sends the service request message *** */
    				 communication_socket_file_descriptor=monitored_set_of_filedescriptors[i];

    				 /* *** memset() C library function ****
    				  * ***************************************
    				  * memset() is used to fill a block of memory with a particular values
    				  * void *memset(void *ptr,int x, size_t n);
    				  * void * means void pointer and means any type of pointer
    				  * ptr ==> starting address of memory to be filled
    				  * x ==> the value to be filled
    				  * n ==> number of bytes to be filled starting from memory address ptr
    				  */

    				 memset(data_buffer,0,sizeof(data_buffer));

    				 sent_received_bytes=recvfrom(communication_socket_file_descriptor,(char *) data_buffer, sizeof(data_buffer),0, (struct sockaddr *)&client_address,&address_length);

    				 printf("The server has received  %d bytes from the client %s at port %u.\n", sent_received_bytes, inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));

    				 if(sent_received_bytes==0)
    				 {
    					 /* *** If the server receives an empty message from the client., then the server may close the connection and wait for another fresh connection from the same client *** */
    					 close(communication_socket_file_descriptor);
    					 remove_from_monitored_set_of_filescriptors(communication_socket_file_descriptor);
    					 break;
    				 }

    				 test_struct_t *client_data=(test_struct_t *) data_buffer; //data is legitimate, then typecast it to the its format we know it was sent as

    				 /* *** verify if the client sends a special message to the server, in such case server closes the client connection *** */
    				 if(client_data->a==0 && client_data->b==0)
    				 {
    					 close(communication_socket_file_descriptor);
    					 remove_from_monitored_set_of_filescriptors(communication_socket_file_descriptor);

    					 printf("The server has closed the connection with the client %s at port %u.\n", inet_ntoa(client_address.sin_addr),ntohs(client_address.sin_port));
    					 break; //get out of the while infinite loop.
    				 }

    				 result_struct_t result;
    				 result.c=client_data->a+client_data->b;

    				 /* ***  The server replies to the client at this point *** */
    				 sent_received_bytes=sendto(communication_socket_file_descriptor, (char *) &result,sizeof(result_struct_t),0,(struct sockaddr *)&client_address,address_length);

    				 printf("The server has sent %d bytes in reply to the client %s.\n", sent_received_bytes,inet_ntoa(client_address.sin_addr));

    			}
    		}
    	}

    }


}

/* *** Helper functions complete *** */
static void initialize_set_of_filedescriptors()
{
	for(int i=0;i<NO_MAX_CLIENTS;i++)
	{
		monitored_set_of_filedescriptors[i]=-1; //initially, there is nothing in the set of file descriptors. -1 represents the absence of file descriptors in the array of file descriptors.
	}
}

static void add_to_monitored_set_of_filescriptors(int socket_filedescriptor)
{
	for(int i=0;i<NO_MAX_CLIENTS;i++)
	{
		if(monitored_set_of_filedescriptors[i]!=-1)
		{
			continue;
		}

		monitored_set_of_filedescriptors[i]=socket_filedescriptor; //at the moment of finding the first empty place in the array, enter the master socket file descriptor
		break;
	}
}

static void re_initialize_collection_of_filedescriptor(fd_set *pointer_to_fd_set)

{
	//this function copies each element of the array containing the file descriptors to the collection of file descriptors of the special data type fd_set which is watched by the select() system call
	FD_ZERO(pointer_to_fd_set); //discard what is exactly there in the collection

	for(int i=0;i<NO_MAX_CLIENTS;i++)
	{
		if(monitored_set_of_filedescriptors[i]!=-1)
		{
			FD_SET(monitored_set_of_filedescriptors[i],pointer_to_fd_set); //copy each element of the array with file descriptors to the fd_set collection because select() system call only works with fd_set type of arguments
		}
	}
}

static int get_max_filedescriptor()
{
	int max=-1;

	for(int i=0; i<NO_MAX_CLIENTS;i++)
	{
		if(monitored_set_of_filedescriptors[i]>max)
		{
			max=monitored_set_of_filedescriptors[i];
		}
	}

	return max;
}

static void remove_from_monitored_set_of_filescriptors(int socket_filescriptor)
{
	for (int i=0;i<NO_MAX_CLIENTS;i++)
	{
		if(monitored_set_of_filedescriptors[i]!=socket_filescriptor)
		{
			continue;
		}

		monitored_set_of_filedescriptors[i]=-1;
		break;
	}
}
