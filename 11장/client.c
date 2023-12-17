#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define SERVER_IP "127.0.0.1"

int main()
{
  int client_socket;
  struct sockaddr_in server_addr;
  char message[1024];

  // 클라이언트 소켓 생성
  client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (client_socket == -1)
  {
    perror("Socket creation error");
    exit(EXIT_FAILURE);
  }

  // 서버 주소 설정
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

  // 서버에 연결
  if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Connection error");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    printf("Enter message (or 'exit' to quit): ");
    fgets(message, sizeof(message), stdin);

    // 서버에 메시지 전송
    send(client_socket, message, strlen(message), 0);

    if (strcmp(message, "exit\n") == 0)
    {
      break;
    }

    // 서버로부터 응답 받기
    recv(client_socket, message, sizeof(message), 0);
    printf("Server response: %s", message);
  }

  close(client_socket);

  return 0;
}
