#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "사용법: %s <파일 이름>\n", argv[0]); // 올바른 명령행 인수 갯수를 확인하고 사용법 출력
    return 1;
  }

  const char *filename = argv[1];    // 명령행 인수로 전달된 파일 이름
  FILE *file = fopen(filename, "r"); // 파일 열기 (읽기 모드)

  if (file == NULL)
  {
    perror("파일 열기 실패"); // 파일 열기 실패 시 에러 메시지 출력
    return 1;
  }

  char line[MAX_LINE_LENGTH];     // 한 줄을 저장할 버퍼
  char reversed[MAX_LINE_LENGTH]; // 뒤집힌 한 줄을 저장할 버퍼
  size_t line_length;

  while (fgets(line, sizeof(line), file) != NULL) // 파일에서 한 줄씩 읽음
  {
    line_length = strlen(line);

    // 거꾸로 뒤집기
    for (size_t i = 0; i < line_length; i++)
    {
      reversed[i] = line[line_length - 1 - i]; // 문자열을 거꾸로 뒤집음
    }

    // 널 문자 추가
    reversed[line_length] = '\0'; // 뒤집힌 문자열의 끝에 널 문자 추가

    // 화면에 출력
    printf("%s", reversed); // 뒤집힌 문자열을 화면에 출력
  }

  fclose(file); // 파일 닫기
  return 0;
}
