#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include "safeUtil.h"
#include "pdu_functions.h"

int sendPDU(int clientSocket, uint8_t* dataBuffer, int lengthOfData){
    uint8_t pdu_buffer[lengthOfData+2];
    uint16_t lengthOfData_no = lengthOfData;
    //lengthOfData_no = htons(lengthOfData);
    
    memcpy(pdu_buffer, &lengthOfData_no, 2);
    memcpy(pdu_buffer+2, dataBuffer, lengthOfData);

    int bytes_sent = safeSend(clientSocket, pdu_buffer, lengthOfData+2, 0);

    return lengthOfData;
    // return lengthOfData;


}

int recvPDU(int socketNumber, uint8_t* dataBuffer, int bufferSize){
    uint8_t message_length=0;
    safeRecv(socketNumber, &message_length, 2, MSG_WAITALL);
    printf("---Message length: %d\n", message_length);

    // if(bufferSize < message_length){
    //     perror("buffer size");
    //     exit(1);
    // }

    //message_length = ntohs(message_length);
        if(bufferSize < message_length){
        perror("buffer size");
        exit(1);
    }
    int n = safeRecv(socketNumber, dataBuffer, message_length, MSG_WAITALL);
    

    return n;
}

