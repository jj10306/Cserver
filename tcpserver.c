#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVERPORT 18000
#define MAXMESSAGE 2048

int main(int argc, char **argv) {
  int listenfd, connfd, n;
  struct sockaddr_in servaddr;
  uint8_t buff[MAXMESSAGE + 1];
  uint8_t recvmessage[MAXMESSAGE + 1];

  if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("Error creating server socket\n");
    exit(0);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  // host to network, short/long: handles conversion from host byte order to
  // network standard byte order
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERVERPORT);

  if ((bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr))) < 0) {
    printf("Error ninging the socket\n");
    exit(1);
  };

  if ((listen(listenfd, 10)) < 0) {
    printf("error while listening\n");
    exit(1);
  }

  for (;;) {
    //    struct sockaddr_in addr;
    //    socklen_t addr_len;

    printf("waiting for a connection on port %d\n", SERVERPORT);
    fflush(stdout);
    connfd = accept(listenfd, (struct sockaddr *)NULL, NULL);

    memset(recvmessage, 0, MAXMESSAGE);

    while ((n = read(connfd, recvmessage, MAXMESSAGE - 1)) > 0) {
      printf("\n%s\n", recvmessage);

      if (recvmessage[n - 1] == '\n')
        break;
      memset(recvmessage, 0, MAXMESSAGE);
    }
    if (n < 0) {
      printf("Error while reading");
      exit(1);
    }

    snprintf((char *)buff, sizeof(buff), "HTTP/1.0 200 OK\r\n\r\nJello");

    write(connfd, (char *)buff, strlen((char *)buff));
    close(connfd);
  }
}
