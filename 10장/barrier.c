#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct
{
  pthread_mutex_t mutex;    // 뮤텍스
  pthread_cond_t condition; // 조건 변수
  int count;                // 현재 대기 중인 쓰레드 수
  int initial_count;        // 초기 대기 쓰레드 수
} Barrier;

// 배리어 초기화 (동적 할당)
Barrier *barrier_init(int initial_count)
{
  Barrier *barrier = (Barrier *)malloc(sizeof(Barrier));
  if (barrier == NULL)
  {
    perror("배리어 초기화 실패");
    exit(1);
  }

  barrier->count = barrier->initial_count = initial_count;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->condition, NULL);

  return barrier;
}

// 배리어 초기화 (정적 할당)
void barrier_init_static(Barrier *barrier, int initial_count)
{
  barrier->count = barrier->initial_count = initial_count;
  pthread_mutex_init(&barrier->mutex, NULL);
  pthread_cond_init(&barrier->condition, NULL);
}

// 배리어 파괴
void barrier_destroy(Barrier *barrier)
{
  pthread_mutex_destroy(&barrier->mutex);
  pthread_cond_destroy(&barrier->condition);
  free(barrier);
}

// 대기 함수
void barrier_wait(Barrier *barrier)
{
  pthread_mutex_lock(&barrier->mutex);
  barrier->count--;

  if (barrier->count == 0)
  {
    // 모든 쓰레드가 도달한 경우, 대기 중인 모든 쓰레드를 깨우고 재실행
    barrier->count = barrier->initial_count;
    pthread_cond_broadcast(&barrier->condition);
  }
  else
  {
    // 대기
    pthread_cond_wait(&barrier->condition, &barrier->mutex);
  }

  pthread_mutex_unlock(&barrier->mutex);
}

// 테스트용 쓰레드 함수
void *thread_func(void *arg)
{
  Barrier *barrier = (Barrier *)arg;
  printf("쓰레드 시작\n");
  barrier_wait(barrier);
  printf("쓰레드 종료\n");
  return NULL;
}

int main()
{
  int thread_count = 5;
  pthread_t threads[thread_count];
  Barrier *barrier = barrier_init(thread_count);

  for (int i = 0; i < thread_count; i++)
  {
    pthread_create(&threads[i], NULL, thread_func, barrier);
  }

  for (int i = 0; i < thread_count; i++)
  {
    pthread_join(threads[i], NULL);
  }

  barrier_destroy(barrier);

  return 0;
}

// 쓰레드 시작과 종료 메시지가 동시에 출력되는 것은 대기하는 모든 쓰레드가 동시에 깨어나서 재실행되는 것을 의미합니다.
// 즉, 대기하는 모든 쓰레드가 도달할 때까지 배리어에서 대기하다가,
// 모든 쓰레드가 도달하면 함께 깨어나서 다음 작업을 수행하고 다시 시작하는 것이 배리어의 목적입니다.

// 따라서 출력이 "쓰레드 시작"과 "쓰레드 종료"가 번갈아 나타나는 것은 여러 쓰레드가 동시에 작업을 수행하고 종료하는 결과입니다.
// 이것이 대기하는 모든 쓰레드를 깨우고 재실행시키는 것이 맞습니다.