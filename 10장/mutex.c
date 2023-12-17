#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5    // 버퍼 크기
#define PRODUCER_COUNT 2 // 생산자 스레드 수
#define CONSUMER_COUNT 2 // 소비자 스레드 수

int buffer[BUFFER_SIZE]; // 링 버퍼
int in = 0;              // 생산자가 데이터를 넣을 위치
int out = 0;             // 소비자가 데이터를 가져올 위치

pthread_mutex_t mutex; // 뮤텍스

// 생산자 함수
void *producer(void *arg)
{
  for (int i = 0; i < 5; i++) // 아이템을 5번 생산
  {
    int item = rand() % 100; // 랜덤한 아이템 생성

    pthread_mutex_lock(&mutex);             // 뮤텍스 잠금
    while (((in + 1) % BUFFER_SIZE) == out) // 버퍼가 가득 차면 대기
    {
      pthread_mutex_unlock(&mutex);
      usleep(100); // 잠시 대기
      pthread_mutex_lock(&mutex);
    }

    // 아이템을 버퍼에 넣음
    buffer[in] = item;
    printf("생산자: 아이템 %d을(를) 생산함 (인덱스 %d)\n", item, in);
    in = (in + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    usleep(100);                  // 잠시 대기
  }
  pthread_exit(NULL);
}

// 소비자 함수
void *consumer(void *arg)
{
  for (int i = 0; i < 5; i++) // 아이템을 5번 소비
  {
    pthread_mutex_lock(&mutex); // 뮤텍스 잠금
    while (in == out)           // 버퍼가 비어있으면 대기
    {
      pthread_mutex_unlock(&mutex);
      usleep(100); // 잠시 대기
      pthread_mutex_lock(&mutex);
    }

    // 아이템을 버퍼에서 가져옴
    int item = buffer[out];
    printf("소비자: 아이템 %d을(를) 소비함 (인덱스 %d)\n", item, out);
    out = (out + 1) % BUFFER_SIZE;

    pthread_mutex_unlock(&mutex); // 뮤텍스 잠금 해제
    usleep(100);                  // 잠시 대기
  }
  pthread_exit(NULL);
}

int main()
{
  pthread_t producers[PRODUCER_COUNT];
  pthread_t consumers[CONSUMER_COUNT];

  // 뮤텍스 초기화
  pthread_mutex_init(&mutex, NULL);

  // 생산자 스레드 생성
  for (int i = 0; i < PRODUCER_COUNT; i++)
  {
    pthread_create(&producers[i], NULL, producer, NULL);
  }

  // 소비자 스레드 생성
  for (int i = 0; i < CONSUMER_COUNT; i++)
  {
    pthread_create(&consumers[i], NULL, consumer, NULL);
  }

  // 스레드 종료 대기
  for (int i = 0; i < PRODUCER_COUNT; i++)
  {
    pthread_join(producers[i], NULL);
  }
  for (int i = 0; i < CONSUMER_COUNT; i++)
  {
    pthread_join(consumers[i], NULL);
  }

  // 뮤텍스 해제
  pthread_mutex_destroy(&mutex);

  return 0;
}

// 위 코드에서는 뮤텍스를 사용하여 버퍼에 접근을 보호하고,
// 생산자 스레드와 소비자 스레드가 데이터를 동기화하며 접근할 수 있도록 합니다.
// 이로써 생산자 - 소비자 문제가 해결됩니다.
// 5번 동작함