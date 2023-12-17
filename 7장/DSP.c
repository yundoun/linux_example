#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// 재귀적으로 디렉토리 내의 파일과 디렉토리를 깊이 우선 방식으로 나열하는 함수
void listFilesAndDirectoriesDFS(const char *path)
{
  struct dirent *entry;
  DIR *dp = opendir(path); // 디렉토리 포인터 생성

  if (dp == NULL)
  {
    perror("opendir"); // 디렉토리 열기 실패 시 에러 메시지 출력
    return;
  }

  while ((entry = readdir(dp))) // 디렉토리 내 항목들을 반복해서 읽음
  {
    struct stat statbuf;
    char fullpath[1024];

    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name); // 현재 디렉토리와 항목 이름을 합쳐 전체 경로 생성

    if (lstat(fullpath, &statbuf) < 0)
    {
      perror("lstat"); // 파일 정보 읽기 실패 시 에러 메시지 출력
      continue;
    }

    if (S_ISDIR(statbuf.st_mode)) // 현재 항목이 디렉토리인 경우
    {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      {
        continue; // 현재 디렉토리 및 상위 디렉토리 스킵
      }

      printf("디렉토리: %s/\n", fullpath);  // 디렉토리 경로 출력
      listFilesAndDirectoriesDFS(fullpath); // 재귀적으로 하위 디렉토리 탐색
    }
    else
    {
      printf("파일: %s\n", fullpath); // 파일 경로 출력
    }
  }

  closedir(dp); // 디렉토리 포인터 닫기
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <디렉토리 경로>\n", argv[0]); // 올바른 인수 갯수 확인 및 사용법 출력
    return 1;
  }

  const char *path = argv[1]; // 명령줄에서 제공된 디렉토리 경로
  printf("깊이 우선 방식으로 디렉토리 내용을 나열합니다: %s\n", path);
  listFilesAndDirectoriesDFS(path); // 디렉토리 내용을 깊이 우선 방식으로 나열

  return 0;
}
