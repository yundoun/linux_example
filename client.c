#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8080

void start_client()
{
  int sock = 0;
  struct sockaddr_in serv_addr;
  char *http_request = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
  char buffer[1024] = {0};

  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return;
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // Convert IPv4 and IPv6 addresses from text to binary form
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return;
  }

  send(sock, http_request, strlen(http_request), 0);
  printf("HTTP request sent\n");
  read(sock, buffer, 1024);
  printf("%s\n", buffer);

  close(sock);
}

int main()
{
  start_client();
  return 0;
}
