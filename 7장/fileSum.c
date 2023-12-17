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
  DIR *dp = opendir(path);

  if (dp == NULL)
  {
    perror("opendir");
    return 0;
  }

  long long totalSize = 0;

  while ((entry = readdir(dp)))
  {
    struct stat statbuf;
    char fullpath[1024]; // 경로를 저장할 버퍼

    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

    if (lstat(fullpath, &statbuf) < 0)
    {
      perror("lstat");
      continue;
    }

    if (S_ISDIR(statbuf.st_mode))
    {
      // "."와 ".." 디렉토리를 무시합니다.
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      {
        continue;
      }

      // 디렉토리일 경우 디렉토리 크기를 재귀적으로 계산합니다.
      long long subdirSize = calculateDirectorySize(fullpath);
      totalSize += subdirSize;

      printf("디렉토리: %s/ (%lld bytes)\n", fullpath, subdirSize);
    }
    else
    {
      // 파일일 경우 파일 크기를 더합니다.
      long long fileSize = statbuf.st_size;
      totalSize += fileSize;

      printf("파일: %s (%lld bytes)\n", fullpath, fileSize);
    }
  }

  closedir(dp);

  return totalSize;
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <디렉토리 경로>\n", argv[0]);
    return 1;
  }

  const char *path = argv[1];
  printf("디렉토리 내용 크기를 계산합니다: %s\n", path);
  long long totalSize = calculateDirectorySize(path);
  printf("총 크기: %lld bytes\n", totalSize);

  return 0;
}
