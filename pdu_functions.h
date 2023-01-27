#ifndef PDU_FUNCTIONS_H
#define PDU_FUNCTIONS_H

int sendPDU(int clientSocket, uint8_t* dataBuffer, int lengthOfData);
int recvPDU(int socketNumber, uint8_t* dataBuffer, int bufferSize);

#endif