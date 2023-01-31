#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <wait.h>
#include <string.h>
#include "socketTable.h"


sockTable *newSockTable() {
    sockTable *st = malloc(sizeof(sockTable));
    st->tableLength = 0;
    return st;
}

clientSock *newClientSock() {
    clientSock *cs = malloc(sizeof(clientSock));
    cs->socketNum = 0;
    //cs->handleName = malloc(sizeof(char *));
    return cs;
}

void setupSock(clientSock *cs, int socketNum, char *handleName) {
    cs->socketNum = socketNum;
    cs->handleLength = strlen(handleName);
    memcpy(cs->handleName, handleName, strlen(handleName) + 1);
}

void addClientToSockTable(sockTable *st, int socketNum, char *handleName) {
    clientSock *cs = newClientSock();
    setupSock(cs, socketNum, handleName);
    
    if(st->tableLength == 0) {
        st->tail = cs;
    }
    else {
        cs->next = st->tail;
        st->tail = cs;
    }
    st->tableLength++;
}

//possibly close connection?
void removeClientSock(sockTable *st, int socketNum){
    clientSock *prev = st->tail;
    if(prev->socketNum == socketNum){
        st->tail = prev->next;
        st->tableLength--;
        freeClientSock(prev);
    }

    for(int i = 1; i < st->tableLength; i++){
        if(prev->next->socketNum == socketNum){
            clientSock *temp = prev->next;
            prev->next = prev->next->next;
            freeClientSock(temp);
            st->tableLength--;
        }
        prev = prev->next;
    }
}

void freeSockTable(sockTable *st){
    if(st->tableLength==0){
        free(st);
        return;
    }
    clientSock *curr = st->tail;
    int len = st->tableLength;

    for(int i = 0; i < len; i++){
        clientSock *next = curr->next;
        freeClientSock(curr);
        curr = next;
    }
    //freeClientSock(curr);
    free(st);
}

void freeClientSock(clientSock *cs) {
    //free(cs->handleName);
    free(cs);
}

void printSocks(sockTable * st){
    clientSock *curr = st->tail;
    int len = st->tableLength;

    for(int i = 0; i < len; i++){
        clientSock *next = curr->next;
        printf("%d: Header name: %s, header length: %d\n",curr->socketNum, curr->handleName, curr->handleLength);
        curr = next;
    }
}

clientSock *lookupClientHandle(sockTable *st, char *handleName) {
    clientSock *curr = st->tail;
    int len = st->tableLength;

    for(int i = 0; i < len; i++){
        clientSock *next = curr->next;
        if (strcmp(curr->handleName, handleName) == 0) {
            return curr;
        }
        curr = next;
    }
    //handle name is not in sockTable
    return NULL;
}

clientSock *lookupClientSocket(sockTable *st, int socketNum) {
    clientSock *curr = st->tail;
    int len = st->tableLength;

    for(int i = 0; i < len; i++){
        clientSock *next = curr->next;
        if (curr->socketNum == socketNum) {
            return curr;
        }
        curr = next;
    }
    //handle name is not in sockTable
    return NULL;
}

///client Buff needs to be freed outside of function!////

char **listClientHandles(sockTable *st) {
    char **clientBuff = malloc(sizeof(char *) * st->tableLength);
    clientSock *curr = st->tail;
    int len = st->tableLength;

    for(int i = 0; i < len; i++){
        clientSock *next = curr->next;
        clientBuff[i] = curr->handleName;
        curr = next;
    }
    return clientBuff;
}

char **checkValidHandles(sockTable *st, char *handleList[], int numHandles) {
    char **invalidClientBuff = malloc(sizeof(char *) * st->tableLength);
    int invalidHandles = 0;

    for(int i = 0; i < numHandles; i++) {
        if ((lookupClientHandle(st, handleList[i])) == NULL) {
            invalidClientBuff[invalidHandles] = handleList[i];
            invalidHandles++;
        }
    }
    return invalidClientBuff;
}



int checkValidity(sockTable *st, int socketNum) {
    clientSock *cs = lookupClientSocket(st, socketNum);
    return cs->validFlag;
}

void setValidity(sockTable *st, int socketNum, int validNum) {
    // 0 is valid 1 is not
    if (validNum != 0 || validNum != 1) {
        validNum = 1; // can't set to other than 0/1 therefore 1 is default in this case
    }
    clientSock *cs = lookupClientSocket(st, socketNum);
    cs->validFlag = validNum;
}

int checkConnection(sockTable *st, int socketNum) {
    clientSock *cs = lookupClientSocket(st, socketNum);
    return cs->connectionFlag;
}

void setConnection(sockTable *st, int socketNum, int connNum) {
    // 0 is valid 1 is not
    if (connNum != 0 || connNum != 1) {
        connNum = 1; // can't set to other than 0/1 therefore 1 is default in this case
    }
    clientSock *cs = lookupClientSocket(st, socketNum);
    cs->connectionFlag = connNum;
}

int getTableLength(sockTable *st) {
    return st->tableLength;
}

int getHandleLength(sockTable *st, int socketNum) {
    clientSock *cs = lookupClientSocket(st, socketNum);
    return cs->handleLength;
}

int getSocketNum(sockTable *st, char *handleName) {
    clientSock *cs = lookupClientHandle(st, handleName);
    return cs->socketNum;
}
char* getHandleName(sockTable *st, int socketNum){  
    clientSock *curr = st->tail;
    int len = st->tableLength;

    for(int i = 0; i < len; i++){
        clientSock *next = curr->next;
        if (curr->socketNum == socketNum) {
            return curr->handleName;
        }
        curr = next;
    }
    //handle name is not in sockTable
    return "-1";
}