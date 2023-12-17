#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

// 재귀적으로 디렉토리 내의 파일 및 디렉토리의 크기를 계산하는 함수
long long calculateDirectorySize(const char *path)
{
  struct dirent *entry;
  DIR *dp = opendir(path); // 디렉토리 포인터 생성

  if (dp == NULL)
  {
    perror("opendir"); // 디렉토리 열기 실패 시 에러 메시지 출력
    return 0;
  }

  long long totalSize = 0; // 디렉토리 내 총 크기를 저장할 변수

  while ((entry = readdir(dp))) // 디렉토리 내 항목들을 반복해서 읽음
  {
    struct stat statbuf;
    char fullpath[1024]; // 경로를 저장할 버퍼

    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name); // 현재 디렉토리와 항목 이름을 합쳐 전체 경로 생성

    if (lstat(fullpath, &statbuf) < 0)
    {
      perror("lstat"); // 파일 정보 읽기 실패 시 에러 메시지 출력
      continue;
    }

    if (S_ISDIR(statbuf.st_mode)) // 현재 항목이 디렉토리인 경우
    {
      // "."와 ".." 디렉토리를 무시합니다.
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      {
        continue;
      }

      // 디렉토리일 경우 디렉토리 크기를 재귀적으로 계산합니다.
      long long subdirSize = calculateDirectorySize(fullpath); // 하위 디렉토리 크기 계산
      totalSize += subdirSize;                                 // 현재 디렉토리 크기에 추가

      printf("디렉토리: %s/ (%lld bytes)\n", fullpath, subdirSize); // 디렉토리 경로와 크기 출력
    }
    else
    {
      // 파일일 경우 파일 크기를 더합니다.
      long long fileSize = statbuf.st_size; // 파일 크기 읽기
      totalSize += fileSize;                // 현재 디렉토리 크기에 추가

      printf("파일: %s (%lld bytes)\n", fullpath, fileSize); // 파일 경로와 크기 출력
    }
  }

  closedir(dp); // 디렉토리 포인터 닫기

  return totalSize; // 디렉토리 내 총 크기 반환
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <디렉토리 경로>\n", argv[0]); // 올바른 인수 갯수 확인 및 사용법 출력
    return 1;
  }

  const char *path = argv[1]; // 명령줄에서 제공된 디렉토리 경로
  printf("디렉토리 내용 크기를 계산합니다: %s\n", path);
  long long totalSize = calculateDirectorySize(path); // 디렉토리 내용 크기 계산
  printf("총 크기: %lld bytes\n", totalSize);         // 총 크기 출력

  return 0;
}
