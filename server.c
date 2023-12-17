#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

void start_server()
{
  int server_fd, new_socket;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[1024] = {0};
  char *json_response = "HTTP/1.1 200 OK\nContent-Type: application/json\n\n{\"message\": \"Hello, world!\"}";

  // 소켓 파일 디스크립터를 생성
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("socket failed"); // 소켓 생성 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 포트 재사용 옵션 설정 (SO_REUSEADDR 및 SO_REUSEPORT)
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
  {
    perror("setsockopt"); // 소켓 옵션 설정 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 서버 주소 설정
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // 소켓을 네트워크 주소와 포트에 바인딩
  if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
  {
    perror("bind failed"); // 바인딩 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 연결 요청을 수신하기 위해 소켓을 대기 상태로 설정
  if (listen(server_fd, 3) < 0)
  {
    perror("listen"); // 대기 모드 설정 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 클라이언트의 연결을 수락
  if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
  {
    perror("accept"); // 연결 수락 실패 시 오류 메시지 출력
    exit(EXIT_FAILURE);
  }

  // 소켓으로부터 요청 데이터 읽기
  read(new_socket, buffer, 1024);
  printf("%s\n", buffer);

  // JSON 응답 보내기
  send(new_socket, json_response, strlen(json_response), 0);
  printf("JSON response sent\n");

  // 소켓 및 서버 소켓 닫기
  close(new_socket);
  close(server_fd);
}

int main()
{
  start_server(); // 서버 시작
  return 0;
}

// 이 코드는 간단한 HTTP 서버를 구현한 것으로,
// 요청을 받으면 "Hello, world!"와 같은 JSON 응답을 클라이언트에게 보내는 역할을 합니다.
// 코드 내에 주석을 추가하여 각 부분의 동작과 역할을 상세히 설명하였습니다.