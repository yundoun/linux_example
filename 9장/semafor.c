#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 5    // 버퍼 크기
#define PRODUCER_COUNT 2 // 생산자 스레드 수
#define CONSUMER_COUNT 2 // 소비자 스레드 수

int buffer[BUFFER_SIZE];  // 링 버퍼
sem_t empty, full, mutex; // 세마포어

int in = 0;  // 생산자가 데이터를 넣을 위치
int out = 0; // 소비자가 데이터를 가져올 위치

// 생산자 함수
void *producer(void *arg)
{
  int item;
  for (int i = 0; i < 5; i++) // 아이템을 5번 생산
  {
    item = rand() % 100; // 랜덤한 아이템 생성
    sem_wait(&empty);    // empty 세마포어를 감소시킴
    sem_wait(&mutex);    // 공유 데이터에 접근하는 부분 뮤텍스로 보호

    // 아이템을 버퍼에 넣음
    buffer[in] = item;
    printf("생산자 %d: 아이템 %d을 생산함 (인덱스 %d)\n", *((int *)arg), item, in);
    in = (in + 1) % BUFFER_SIZE;

    sem_post(&mutex); // 뮤텍스 잠금 해제
    sem_post(&full);  // full 세마포어를 증가시킴
    sleep(1);         // 생산하는데 1초 소요
  }
  pthread_exit(NULL);
}

// 소비자 함수
void *consumer(void *arg)
{
  int item;
  for (int i = 0; i < 5; i++) // 아이템을 5번 소비
  {
    sem_wait(&full);  // full 세마포어를 감소시킴
    sem_wait(&mutex); // 공유 데이터에 접근하는 부분 뮤텍스로 보호

    // 아이템을 버퍼에서 가져옴
    item = buffer[out];
    printf("소비자 %d: 아이템 %d을 소비함 (인덱스 %d)\n", *((int *)arg), item, out);
    out = (out + 1) % BUFFER_SIZE;

    sem_post(&mutex); // 뮤텍스 잠금 해제
    sem_post(&empty); // empty 세마포어를 증가시킴
    sleep(2);         // 소비하는데 2초 소요
  }
  pthread_exit(NULL);
}

int main()
{
  pthread_t producers[PRODUCER_COUNT];
  pthread_t consumers[CONSUMER_COUNT];
  int producer_ids[PRODUCER_COUNT];
  int consumer_ids[CONSUMER_COUNT];

  // 세마포어 초기화
  sem_init(&empty, 0, BUFFER_SIZE);
  sem_init(&full, 0, 0);
  sem_init(&mutex, 0, 1);

  // 생산자 스레드 생성
  for (int i = 0; i < PRODUCER_COUNT; i++)
  {
    producer_ids[i] = i + 1;
    pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
  }

  // 소비자 스레드 생성
  for (int i = 0; i < CONSUMER_COUNT; i++)
  {
    consumer_ids[i] = i + 1;
    pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
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

  // 세마포어 해제
  sem_destroy(&empty);
  sem_destroy(&full);
  sem_destroy(&mutex);

  return 0;
}

// 이 코드는 뮤텍스와 세마포어를 사용하여 생산자 - 소비자 문제를 해결하는 예제입니다.
// 프로그램은 PRODUCER_COUNT 개수의 생산자 스레드와 CONSUMER_COUNT 개수의 소비자 스레드를 생성하고,
// 링 버퍼에 데이터를 생산자가 넣고 소비자가 가져가는 동작을 반복합니다.
// 세마포어 empty, full, mutex를 사용하여 스레드 간 동기화를 제어합니다.