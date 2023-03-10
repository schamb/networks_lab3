/******************************************************************************
* myServer.c
* 
* Writen by Prof. Smith, updated Jan 2023
* Use at your own risk.  
*
*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdint.h>
#include <poll.h>

#include "networks.h"
#include "safeUtil.h"
#include "pdu_functions.h"
#include "pollLib.h"

#define MAXBUF 1024
#define DEBUG_FLAG 1

void recvFromClient(int clientSocket);
int checkArgs(int argc, char *argv[]);
void serverControl(int portNumber);
void addNewClient(int mainServerSocket);
void processClient(int clientSocket);


int main(int argc, char *argv[])
{
	//setup port number
	int portNumber = 0;
	portNumber = checkArgs(argc, argv);
	
	serverControl(portNumber);
	
	return 0;
}

void recvFromClient(int clientSocket)
{
	uint8_t dataBuffer[MAXBUF];
	int messageLen = 0;
	
	
	//now get the data from the client_socket
	if ((messageLen = recvPDU(clientSocket, dataBuffer, MAXBUF)) < 0)
	{
		perror("recv call");
		exit(-1);
	}

	if (messageLen > 0)
	{
		printf("Message received, length: %d Data: %s\n", messageLen, dataBuffer);
	}
	else
	{
		printf("Connection closed by other side\n");
		removeFromPollSet(clientSocket);
		close(clientSocket);
	}
	
	
}

int checkArgs(int argc, char *argv[])
{
	// Checks args and returns port number
	int portNumber = 0;

	if (argc > 2)
	{
		fprintf(stderr, "Usage %s [optional port number]\n", argv[0]);
		exit(-1);
	}
	
	if (argc == 2)
	{
		portNumber = atoi(argv[1]);
	}
	
	return portNumber;
}


void serverControl(int portNumber){

	int mainServerSocket = 0;   //socket descriptor for the server socket

	//setup poll set
	setupPollSet();

	//at the beginning of the program, add main server socket to poll list
	//create the server socket
	mainServerSocket = tcpServerSetup(portNumber);
	addToPollSet(mainServerSocket);


	while (1){ 
		//Call poll() (e.g. use my poll library) 
		int ready_socket = pollCall(-1);

		//If poll() returns the main server socket, call addNewClient().
		if(ready_socket == mainServerSocket) addNewClient(mainServerSocket); 

		//If poll() returns a client socket, call processClient(). 
		else if(ready_socket != -1) processClient(ready_socket);

		//timeout
		else fprintf(stderr, "timeout\n");
	}
	
	//close 
	close(mainServerSocket);

}

void addNewClient(int mainServerSocket){
	int clientSocket = 0;   //socket descriptor for the client socket

	//accept client
	clientSocket = tcpAccept(mainServerSocket, DEBUG_FLAG);

	//add to poll set
	addToPollSet(clientSocket);
}

void processClient(int clientSocket){
	//recvPDU
	recvFromClient(clientSocket);
}
