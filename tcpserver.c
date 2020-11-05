#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define SERVERPORT 18000
#define MAXMESSAGE 2048

char *get_filename(char *request, int *len);
int test_get_filename();

int test_get_filename() {
  int len = -1;
  char r1[] = "GET /file1 other stuff";
  char res1[] = "/file1";
  char *actual1 = get_filename(r1, &len);
  printf("Request 1: %s, filename 1: %s, Actual: %s, Actual len %d", r1, res1,
         actual1, len);
  return strcmp(actual1, res1);
}
// takes raw HTTP request  returns the desired file name and the length through
// the len param return the length of the filename or -1 if an error occurrs
char *get_filename(char *request, int *len) {
  // important assumption is that the request is well formed  and the filename
  // doesn't have spaces
  int start, end;
  start = end = -1;
  int i = 0;
  // "GET /filename <other stuff>"
  // consider using strchr instead of doing this manually
  while (request[i] != '\r') {
    if (request[i] == ' ' && start == -1) {
      start = i;
    } else if (request[i] == ' ' && end == -1) {
      end = i;
      break;
    }
    i++;
  }
  if (start == end)
    return NULL;
  int length = end - start - 1;

  char *filename;
  if ((filename = calloc(length + 1, sizeof(*filename))) == NULL)
    return NULL;

  memcpy(filename, &request[start + 1], length);
  filename[end - 1] = '\0';
  *len = length;

  return filename;
}

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
