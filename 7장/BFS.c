#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 1024

// 큐를 구현하기 위한 구조체 정의
struct Queue
{
  char *data[MAX_QUEUE_SIZE];
  int front;
  int rear;
};

// 큐 초기화 함수
void initQueue(struct Queue *q)
{
  q->front = 0;
  q->rear = 0;
}

// 큐에 데이터 추가 함수
void enqueue(struct Queue *q, const char *data)
{
  q->data[q->rear] = strdup(data);
  q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
}

// 큐에서 데이터 추출 함수
const char *dequeue(struct Queue *q)
{
  const char *data = q->data[q->front];
  q->front = (q->front + 1) % MAX_QUEUE_SIZE;
  return data;
}

// 큐가 비어 있는지 확인하는 함수
int isQueueEmpty(struct Queue *q)
{
  return q->front == q->rear;
}

// 재귀적으로 디렉토리 내의 파일과 디렉토리를 넓이 우선 방식으로 나열하는 함수
void listFilesAndDirectoriesBFS(const char *path)
{
  struct dirent *entry;
  DIR *dp = opendir(path);

  if (dp == NULL)
  {
    perror("opendir");
    return;
  }

  struct Queue queue;
  initQueue(&queue);

  enqueue(&queue, path);

  while (!isQueueEmpty(&queue))
  {
    const char *currentDir = dequeue(&queue);
    dp = opendir(currentDir);

    while ((entry = readdir(dp)))
    {
      struct stat statbuf;
      char fullpath[1024];

      snprintf(fullpath, sizeof(fullpath), "%s/%s", currentDir, entry->d_name);

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
        enqueue(&queue, fullpath);
      }
      else
      {
        printf("파일: %s\n", fullpath);
      }
    }

    closedir(dp);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <디렉토리 경로>\n", argv[0]);
    return 1;
  }

  const char *path = argv[1];
  printf("넓이 우선 방식으로 디렉토리 내용을 나열합니다: %s\n", path);
  listFilesAndDirectoriesBFS(path);

  return 0;
}
