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
  char *data[MAX_QUEUE_SIZE]; // 큐 요소를 저장하는 배열
  int front;                  // 큐의 앞부분 인덱스
  int rear;                   // 큐의 뒷부분 인덱스
};

// 큐 초기화 함수
void initQueue(struct Queue *q)
{
  q->front = 0; // 큐의 앞부분 초기화
  q->rear = 0;  // 큐의 뒷부분 초기화
}

// 큐에 데이터 추가 함수
void enqueue(struct Queue *q, const char *data)
{
  q->data[q->rear] = strdup(data);          // 큐의 뒷부분에 데이터 추가
  q->rear = (q->rear + 1) % MAX_QUEUE_SIZE; // 큐의 뒷부분 인덱스 증가 (원형 큐)
}

// 큐에서 데이터 추출 함수
const char *dequeue(struct Queue *q)
{
  const char *data = q->data[q->front];       // 큐의 앞부분 데이터 추출
  q->front = (q->front + 1) % MAX_QUEUE_SIZE; // 큐의 앞부분 인덱스 증가 (원형 큐)
  return data;
}

// 큐가 비어 있는지 확인하는 함수
int isQueueEmpty(struct Queue *q)
{
  return q->front == q->rear; // 큐의 앞과 뒤 인덱스가 같으면 큐가 비어있음
}

// 재귀적으로 디렉토리 내의 파일과 디렉토리를 넓이 우선 방식으로 나열하는 함수
void listFilesAndDirectoriesBFS(const char *path)
{
  struct dirent *entry;
  DIR *dp = opendir(path); // 디렉토리 열기

  if (dp == NULL)
  {
    perror("opendir"); // 디렉토리 열기 실패 시 에러 메시지 출력
    return;
  }

  struct Queue queue;
  initQueue(&queue); // 큐 초기화

  enqueue(&queue, path); // 시작 디렉토리 경로를 큐에 추가

  while (!isQueueEmpty(&queue)) // 큐가 비어 있지 않은 동안 반복
  {
    const char *currentDir = dequeue(&queue); // 큐에서 디렉토리 경로 추출
    dp = opendir(currentDir);                 // 현재 디렉토리 열기

    while ((entry = readdir(dp)))
    {
      struct stat statbuf;
      char fullpath[1024];

      snprintf(fullpath, sizeof(fullpath), "%s/%s", currentDir, entry->d_name);

      if (lstat(fullpath, &statbuf) < 0)
      {
        perror("lstat"); // 파일 정보 읽기 실패 시 에러 메시지 출력
        continue;
      }

      if (S_ISDIR(statbuf.st_mode))
      {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
        {
          continue; // 현재 디렉토리 및 상위 디렉토리 스킵
        }

        printf("디렉토리: %s/\n", fullpath); // 디렉토리 경로 출력
        enqueue(&queue, fullpath);           // 하위 디렉토리 큐에 추가
      }
      else
      {
        printf("파일: %s\n", fullpath); // 파일 경로 출력
      }
    }

    closedir(dp); // 현재 디렉토리 닫기
  }
}

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    fprintf(stderr, "Usage: %s <디렉토리 경로>\n", argv[0]); // 올바른 인수 갯수 확인 및 사용법 출력
    return 1;
  }

  const char *path = argv[1]; // 명령줄에서 제공된 디렉토리 경로
  printf("넓이 우선 방식으로 디렉토리 내용을 나열합니다: %s\n", path);
  listFilesAndDirectoriesBFS(path); // 디렉토리 내용을 넓이 우선 방식으로 나열

  return 0;
}
