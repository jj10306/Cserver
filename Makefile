CC=gcc
CFLAGS= -g -Wall

all: tcpclient

tcpclient: tcpclient.c
	$(CC) $(CFLAGS) -o tcpclient tcpclient.c

tcpserver: tcpserver.c
	$(CC) $(CFLAGS) -o tcpserver tcpserver.c

clean:
	rm tcpclient tcpserver 
