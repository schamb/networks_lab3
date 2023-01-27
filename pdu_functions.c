#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "safeUtil.h"
#include "pdu_functions.h"


/*
int sendPDU(int clientSocket, uint8_t * dataBuffer, int lengthOfData) {
    uint8_t buffer[1500];
    int headerLength = htons(lengthOfData + 2);
    memcpy(buffer, &headerLength, 2);
    memcpy(buffer + 2, dataBuffer, lengthOfData);
    safeSend(clientSocket, buffer, headerLength, 0);
}
*/

int sendPDU(int clientSocket, uint8_t* dataBuffer, int lengthOfData){
    uint8_t pdu_buffer[lengthOfData+2];
    memcpy(pdu_buffer, &lengthOfData, 2);
    memcpy(pdu_buffer+2, dataBuffer, lengthOfData);

    safeSend(clientSocket, pdu_buffer, lengthOfData+2, 0);

    return lengthOfData;


}

int recvPDU(int socketNumber, uint8_t* dataBuffer, int bufferSize){
    uint8_t message_length;
    safeRecv(socketNumber, &message_length, 2, MSG_WAITALL);
    if(message_length <= 0){
        fprintf(stderr, "message length");
        exit(1);
    }
    int n = safeRecv(socketNumber, dataBuffer, message_length, MSG_WAITALL);

    return n;
}

