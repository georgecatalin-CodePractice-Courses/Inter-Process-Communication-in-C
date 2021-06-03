/*
 * tcp_client.c
 *
 * TCP Client Code Implementation. Steps to follow
 * ************************************************
 * 1. Initialize variables
 * 2. Specify Server Credentials
 * 3. Create a Communication Socket
 * 4. Send a connection initiation request to the server
 * 5. Send data to the Server
 * 6. Receive Response
 * 7. Close the Connection (optional)
 * 8. Go to 6 to communicate more with the same server during the same connection lifetime, if you did not closed connection
 ****************************************************
 *
 *
 * Created on: Jun 3, 2021
 * Author: George Calin
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <memory.h>
#include "common.h"

#define SERVER_IP_ADDRESS "192.168.55.208"
#define SERVER_PORT 2021

test_struct_t client_data;
result_struct_t result_data; //the variable to store the response from the server

/* *** prototyping the helper functions *** */
static void run_tcp_client_communication(void);

int main(int argc, char** argv)
{
	run_tcp_client_communication();

	printf("The client application quits now...\n");
	return 0;
}


static void run_tcp_client_communication(void)
{
	/* *** Initialize the variables needed for the TCP communication *** */
	int socket_filedescriptor=0;
	int sent_received_bytes=0;

	int address_length=sizeof(struct sockaddr);

	struct sockaddr_in server_address; //declare the struct container to store the details about the server (IP and Port)

	/* *** Specify the server credentials *** */
	/*  Detail the server information using the struct variable defined at the previous step
	 *  */
	server_address.sin_family=AF_INET; //IPv4 kind of address
	server_address.sin_port=SERVER_PORT;

	/* *** convert the IP Address from IPv4 format to the format understood by the struct 'sockaddr_in' *** */
	struct hostent *server_host;
	server_host=(struct hostent *)gethostbyname(SERVER_IP_ADDRESS);

	/* *** get the address of the server *** */
	server_address.sin_addr=*((struct in_addr *) server_host->h_addr_list[0]); //dereference the pointer, get the value stored at that memory address

	/* *** Create a communication socket *** */
	socket_filedescriptor=socket(AF_INET,SOCK_STREAM, IPPROTO_TCP);

	/* *** Send connection initiation request message to the server using connect() system call *** */
	/* Connect with the server */
	connect(socket_filedescriptor,(struct sockaddr *) &server_address,address_length);

	//at this point the connection has been established with the TCP Server and the client is in the position to exchange data with the TCP Server using sendto() system call

PROMPT_THE_USER: //This is a C label
	printf("Enter first number a: ? \n");
	scanf("%u", &client_data.a);
	printf("Enter the second number b: ?\n");
	scanf("%u",&client_data.b);

	/* *** Send data to the server *** */
	sent_received_bytes=sendto(socket_filedescriptor,&client_data,sizeof(test_struct_t),0,(struct sockaddr *) &server_address, address_length);

	printf("Number of bytes sent: %d\n",sent_received_bytes);

    /* ***  Receive response from the server *** */
	/* recvfrom() is a blocking system call, it means that the next line of code from the client program will not be executed until data arrives on the socket filescriptor from the server */
	sent_received_bytes=recvfrom(socket_filedescriptor,(char *) &result_data, sizeof(result_struct_t),0, (struct sockaddr *) &server_address,&address_length);

	printf("Number of bytes received is %d \n",sent_received_bytes);

	printf("The result received is %u.\n", result_data.c);

	/* *** Communicate more with the same server or close the connection *** */
	goto PROMPT_THE_USER;

}
