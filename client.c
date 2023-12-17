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

  // 소켓 생성
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    printf("\n Socket creation error \n");
    return;
  }

  // 서버 주소 설정
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);

  // IPv4 주소를 이진 형식으로 변환하여 설정
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
  {
    printf("\nInvalid address/ Address not supported \n");
    return;
  }

  // 서버에 연결
  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
    printf("\nConnection Failed \n");
    return;
  }

  // HTTP 요청을 서버에 보냄
  send(sock, http_request, strlen(http_request), 0);
  printf("HTTP request sent\n");

  // 서버로부터 응답을 읽어와 출력
  read(sock, buffer, 1024);
  printf("%s\n", buffer);

  // 소켓 닫기
  close(sock);
}

int main()
{
  start_client(); // 클라이언트 시작
  return 0;
}

// 이 코드는 간단한 HTTP 클라이언트를 구현한 것으로, 지정된 서버에 HTTP
// GET 요청을 보내고 응답을 수신하여 출력합니다.코드 내에 주석을 추가하
// 여 각 부분의 동작과 역할을 상세히 설명하였습니다.