#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t got_alarm = 0;

// 시그널 핸들러 함수
void alarmHandler(int signum)
{
  got_alarm = 1;
}

// 사용자 정의 sleep 함수
unsigned int customSleep(unsigned int seconds)
{
  struct sigaction sa;
  sa.sa_handler = alarmHandler;
  sa.sa_flags = 0;
  sigaction(SIGALRM, &sa, NULL);

  // alarm 시그널을 설정하여 주어진 시간만큼 대기합니다.
  alarm(seconds);

  // 시그널이 도착할 때까지 대기합니다.
  while (!got_alarm)
  {
    pause();
  }

  // alarm 시그널을 해제합니다.
  alarm(0);

  return seconds;
}

int main()
{
  printf("Sleeping for 5 seconds...\n");
  unsigned int sleepTime = customSleep(5);
  printf("Woke up after %u seconds.\n", sleepTime);

  return 0;
}
