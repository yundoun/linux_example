// ▶ 텍스트 파일의 내용을 역으로 출력하기
// 주어진 텍스트 파일의 내용을 읽어서 그 내용을 화면에 거꾸로 출력하는 프로그램을 작성하시오.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "%stest.c\n", argv[0]);
    return 1;
  }

  const char *filename = argv[1];
  FILE *file = fopen(filename, "r");

  if (file == NULL)
  {
    perror("파일 열기 실패");
    return 1;
  }

  char line[MAX_LINE_LENGTH];
  char reversed[MAX_LINE_LENGTH];
  size_t line_length;

  while (fgets(line, sizeof(line), file) != NULL)
  {
    line_length = strlen(line);

    // 거꾸로 뒤집기
    for (size_t i = 0; i < line_length; i++)
    {
      reversed[i] = line[line_length - 1 - i];
    }

    // 널 문자 추가
    reversed[line_length] = '\0';

    // 화면에 출력
    printf("%s", reversed);
  }

  fclose(file);
  return 0;
}
