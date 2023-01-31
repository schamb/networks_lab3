#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <arpa/inet.h>
#include "networks.h"
#include "safeUtil.h"
#include "pdu_functions.h"

#define MAXBUF 1024

int sendPDU(int clientSocket, uint8_t* dataBuffer, int lengthOfData){
    uint8_t pdu_buffer[lengthOfData+2];
    uint16_t lengthOfData_no = htons(lengthOfData);
    
    memcpy(pdu_buffer, &lengthOfData_no, 2);
    memcpy(pdu_buffer+2, dataBuffer, lengthOfData);

    safeSend(clientSocket, pdu_buffer, lengthOfData+2, 0);

    return lengthOfData;

}

int recvPDU(int socketNumber, uint8_t* dataBuffer, int bufferSize){
    uint16_t message_length;
    safeRecv(socketNumber, (uint8_t*)&message_length, 2, MSG_WAITALL);
    message_length = ntohs(message_length);
    if (message_length > MAXBUF){
        return 0;
    }
    //printf("---Message length: %u\n", message_length);
    int n = safeRecv(socketNumber, dataBuffer, message_length, MSG_WAITALL);
    // if (n != message_length) {
    //     perror("message length error\n");
    //     return 0;
    // }
    return n;
}

