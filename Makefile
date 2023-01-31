# Makefile for CPE464 tcp test code
# written by Hugh Smith - April 2019

CC= gcc
CFLAGS= -g -Wall
LIBS = 

OBJS = networks.o gethostbyname.o pollLib.o safeUtil.o pdu_functions.o socketTable.o

all:   cclient server

cclient: cclient.c $(OBJS)
	$(CC) $(CFLAGS) -pedantic -std=c99 -o cclient cclient.c $(OBJS) $(LIBS)

server: server.c $(OBJS)
	$(CC) $(CFLAGS) -pedantic -std=c99 -o server server.c $(OBJS) $(LIBS)

socketTable.o: socketTable.c
	$(CC) -c $(CFLAGS) -pedantic -std=c99 -o socketTable.o socketTable.c 

pdu_functions: pdu_functions.c $(OBJS)
	$(CC) $(CFLAGS) -pedantic -std=c99 -o pdu_functions pdu_functions.c $(OBJS) $(LIBS)

.c.o:
	gcc -c $(CFLAGS) $< -o $@ $(LIBS)

cleano:
	rm -f *.o

clean:
	rm -f server client *.o




