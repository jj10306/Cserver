#include <sys/socket.h>
#include <stdio.h>

#define SERVERPORT 80
#define MAXMESSAGE 2048

int main(int argc, char **argv)
{
    int sockfd, n;
    int sendbytes;
    struct sockaddr_in servaddr;
    char sendmessage[MAXMESSAGE];
    char recvmessage[MAXMESSAGE];

    if (argc != 2) 
    {
        printf("Incorrect usage");
        exit(0)
    }
    


}
