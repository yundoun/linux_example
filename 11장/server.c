#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 12345
#define MAX_CLIENTS 10

void *handle_client(void *arg); // 클라이언트를 처리하는 함수 정의

int main()
{
  int server_socket, client_socket;
  struct sockaddr_in server_addr, client_addr;
  socklen_t client_addr_len;
  pthread_t thread_id;

  // 서버 소켓 생성
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket == -1)
  {
    perror("Socket creation error"); // 소켓 생성 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 서버 주소 설정
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  // 소켓 바인딩
  if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
  {
    perror("Bind error"); // 소켓 바인딩 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 리스닝 모드로 소켓 설정
  if (listen(server_socket, MAX_CLIENTS) == -1)
  {
    perror("Listen error"); // 리스닝 모드 설정 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  printf("Server listening on port %d...\n", PORT);

  while (1)
  {
    client_addr_len = sizeof(client_addr);
    // 클라이언트 연결 수락
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
    if (client_socket == -1)
    {
      perror("Accept error"); // 클라이언트 연결 수락 실패 시 오류 메시지 출력
      continue;
    }

    // 클라이언트를 처리하기 위한 스레드 생성
    if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_socket) != 0)
    {
      perror("Thread creation error"); // 스레드 생성 실패 시 오류 메시지 출력
      continue;
    }

    printf("Client connected\n");
  }

  close(server_socket);

  return 0;
}

void *handle_client(void *arg)
{
  int client_socket = *((int *)arg);
  char buffer[1024];
  ssize_t bytes_received;

  while (1)
  {
    // 클라이언트로부터 데이터 수신
    bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
    if (bytes_received <= 0)
    {
      break;
    }

    // 수신한 데이터를 클라이언트에게 다시 전송
    send(client_socket, buffer, bytes_received, 0);
  }

  close(client_socket);
  printf("Client disconnected\n");

  pthread_exit(NULL); // 스레드 종료
}

// 서버 소켓 생성 및 초기화.서버 주소 설정 및 소켓 바인딩.리스닝 모드로 소켓 설정.클라이언트 연결을 수락하고,
// 각 클라이언트를 처리하는 스레드를 생성.클라이언트와 데이터를 주고받는 handle_client 함수 정의.
// 클라이언트와의 연결이 종료되면 스레드가 종료되고,클라이언트 연결을 수락하고 처리하는 과정이 반복됩니다.