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
#include <signal.h>

#include "networks.h"
#include "safeUtil.h"
#include "pdu_functions.h"
#include "pollLib.h"
#include "socketTable.h"

#define MAXBUF 1024
#define DEBUG_FLAG 1
static int mainServerSocket = 0;

void recvFromClient(sockTable *st, int clientSocket);
int checkArgs(int argc, char *argv[]);
void serverControl(sockTable *st, int portNumber);
void addNewClient(); //(int mainServerSocket)
void processClient(sockTable *st, int clientSocket);
void sigHandler(int signo);
void sendMessageToNextClient(int clientSocket, uint8_t *dataBuffer, int messageLen);
char *getMessageType();


int main(int argc, char *argv[])
{
	signal(SIGINT, sigHandler);
	//setup port number
	int portNumber = 0;
	portNumber = checkArgs(argc, argv);
	struct sockTable *st = newSockTable();
	serverControl(st, portNumber);
	freeSockTable(st);
	return 0;
}

void recvFromClient(sockTable *st, int clientSocket)
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
		sendMessageToNextClient(clientSocket, dataBuffer, messageLen);
		//printf("Message received, length: %d Data: %s\n", messageLen, dataBuffer);
	}
	else
	{
		printf("Connection closed by other side\n");
		removeFromPollSet(clientSocket);
		removeClientSock(st, clientSocket); //removes from st and frees cs
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

char *getMessageType() {
	return NULL;
}


void serverControl(sockTable *st, int portNumber){

	//int mainServerSocket = 0;   //socket descriptor for the server socket

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
		if(ready_socket == mainServerSocket) {
			addNewClient(mainServerSocket);
			addClientToSockTable(st, mainServerSocket, getHandleName(st, mainServerSocket));
		} 

		//If poll() returns a client socket, call processClient(). 
		else if(ready_socket != -1) {
			processClient(st, ready_socket);
			//char* m_type = getMessageType(); 

			/* 
			getFlag() -> sendMessage(), listHandles(), exitClient()
			sendMessage() -> checkValidHandles(), checkMessageType(), sendPDU()
			*/
		}

		//timeout
		else {
			fprintf(stderr, "timeout\n");
		}
	}
	// removeFromPollSet(mainServerSocket);
	// close(mainServerSocket);
}

void addNewClient() { //(int mainServerSocket)
	int clientSocket = 0;   //socket descriptor for the client socket

	//accept client
	clientSocket = tcpAccept(mainServerSocket, DEBUG_FLAG);

	//add to poll set
	addToPollSet(clientSocket);
}

void processClient(sockTable *st, int clientSocket){
	//recvPDU
	recvFromClient(st, clientSocket);
}


void sigHandler(int signo) {
    signal(signo, SIG_IGN);
	removeFromPollSet(mainServerSocket);
	freePollSet();
	close(mainServerSocket);	
}

//process message
//pass along to next client via socketnum
void sendMessageToNextClient(int clientSocket, uint8_t *dataBuffer, int messageLen)
{
	//uint8_t sendBuf[MAXBUF];   //data buffer
	//int sendLen = 0;        //amount of data to send
	int sent = 0;            //actual amount of data sent/* get the data and send it   */

	sent = sendPDU(clientSocket, dataBuffer, messageLen);
	if (sent < 0)
	{
		perror("send call");
		exit(-1);
	}

	//printf("Amount of data sent is: %d\n", sent);
	
}