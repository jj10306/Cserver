CC=gcc
CFLAGS= -g -Wall

all: tcpclient

tcpclient: tcpclient.c
	$(CC) $(CFLAGS) -o tcpclient tcpclient.c

clean:
	rm tcpclient 
