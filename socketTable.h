#ifndef SOCKETTABLE_H
#define SOCKETTABLE_H

#include<sys/types.h>

typedef struct clientSock {
    struct clientSock *next;
    int socketNum;
    char handleName[100];
    int handleLength;
    int validFlag;
    int connectionFlag;
} clientSock;

typedef struct sockTable {
    struct clientSock *tail;
    int tableLength; //num items
} sockTable;

struct sockTable *newSockTable();
struct clientSock *newClientSock();
void setupSock(struct clientSock *cs, int socketNum, char *handleName);
void addClientToSockTable(sockTable *st, int socketNum, char *handleName);
void removeClientSock(sockTable *st, int socketNum);
void freeSockTable(sockTable *st);
void freeClientSock(struct clientSock *cs);
void printSocks(sockTable *st);
clientSock *lookupClientSocket(sockTable *st, int socketNum);
clientSock *lookupClientHandle(sockTable *st, char *handleName);
char **listClientHandles(sockTable *st);
char **checkValidHandles(sockTable *st, char **handleList, int numHandles);
int checkValidity(sockTable *st, int socketNum);
void setValidity(sockTable *st, int socketNum, int validNum);
int checkConnection(sockTable *st, int socketNum);
void setConnection(sockTable *st, int socketNum, int connNum);
int getTableLength(sockTable *st);
int getHandleLength(sockTable *st, int socketNum);
int getSocketNum(sockTable *st, char *handleName);
char* getHandleName(sockTable *st, int socketNum);


#endif