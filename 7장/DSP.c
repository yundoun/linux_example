#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

// 재귀적으로 디렉토리 내의 파일과 디렉토리를 깊이 우선 방식으로 나열하는 함수
void listFilesAndDirectoriesDFS(const char *path)
{
  struct dirent *entry;
  DIR *dp = opendir(path);

  if (dp == NULL)
  {
    perror("opendir");
    return;
  }

  while ((entry = readdir(dp)))
  {
    struct stat statbuf;
    char fullpath[1024];

    snprintf(fullpath, sizeof(fullpath), "%s/%s", path, entry->d_name);

    if (lstat(fullpath, &statbuf) < 0)
    {
      perror("lstat");
      continue;
    }

    if (S_ISDIR(statbuf.st_mode))
    {
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      {
        continue;
      }

      printf("디렉토리: %s/\n", fullpath);
      listFilesAndDirectoriesDFS(fullpath);
    }
    else
    {
      printf("파일: %s\n", fullpath);
    }
  }

  closedir(dp);
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <디렉토리 경로>\n", argv[0]);
    return 1;
  }

  const char *path = argv[1];
  printf("깊이 우선 방식으로 디렉토리 내용을 나열합니다: %s\n", path);
  listFilesAndDirectoriesDFS(path);

  return 0;
}
