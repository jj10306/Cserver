#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVERPORT 80
#define MAXMESSAGE 2048

int main(int argc, char **argv) {

  int sockfd, n;
  int sendbytes;
  // sockaddr_in is a struct specific to IP-based communication, sockaddr is a
  // general socket
  struct sockaddr_in servaddr;
  char sendmessage[MAXMESSAGE];
  char recvmessage[MAXMESSAGE];

  if (argc != 2) {
    printf("Incorrect usage\n");
    exit(1);
  }
  // AF_INET internet socket, SOCK_STREAM stream socket, 0 for TCP
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error creating socket\n");
    exit(1);
  }

  printf("Address %s\n", argv[1]);
  // zero out the address
  memset(&servaddr, 0, sizeof(servaddr));

  servaddr.sin_family = AF_INET;
  // host to network, short: handles conversion from host byte order to network
  // standard byte order
  servaddr.sin_port = htons(SERVERPORT);

  // takes string IP address and converts it to binary representation and stores
  // it in servadd.sin_addr
  if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
    printf("inet_pton error for %s \n", argv[1]);
    exit(1);
  }

  // connects the socket referred to by sockfd to the address specified by
  // servaddr
  if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
    printf("connection failed\n");
    exit(1);
  }

  sprintf(sendmessage, "GET / HTTP/1.0\r\n\r\n");
  sendbytes = strlen(sendmessage);

  if (write(sockfd, sendmessage, sendbytes) != sendbytes) {
    printf("Error writing data to the socket\n");
    exit(1);
  }

  memset(recvmessage, 0, MAXMESSAGE);

  // read the response from the server
  while ((n = read(sockfd, recvmessage, MAXMESSAGE - 1)) > 0) {
    printf("%d\n", n);
    printf("%s\n", recvmessage);
    memset(recvmessage, 0, MAXMESSAGE);
  }

  if (n < 0) {
    printf("read error\n");
    exit(1);
  }

  exit(0);
}
