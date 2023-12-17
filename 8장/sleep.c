#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t got_alarm = 0; // 시그널 핸들러와 메인 함수 간 통신을 위한 플래그

// 시그널 핸들러 함수
void alarmHandler(int signum)
{
  got_alarm = 1; // 시그널 핸들러에서 플래그를 설정하여 시그널 수신을 알림
}

// 사용자 정의 sleep 함수
unsigned int customSleep(unsigned int seconds)
{
  struct sigaction sa;
  sa.sa_handler = alarmHandler;
  sa.sa_flags = 0;
  sigaction(SIGALRM, &sa, NULL); // SIGALRM 시그널에 대한 핸들러 설정

  // alarm 시그널을 설정하여 주어진 시간만큼 대기합니다.
  alarm(seconds);

  // 시그널이 도착할 때까지 대기합니다.
  while (!got_alarm)
  {
    pause(); // 시그널 도착까지 일시 중지
  }

  // alarm 시그널을 해제합니다.
  alarm(0); // alarm 시그널을 다시 0으로 설정하여 해제

  return seconds; // 대기한 시간을 반환
}

int main()
{
  printf("Sleeping for 5 seconds...\n");
  unsigned int sleepTime = customSleep(5); // 5초 동안 대기
  printf("Woke up after %u seconds.\n", sleepTime);

  return 0;
}

// 이 코드는 customSleep 함수를 사용하여 사용자 정의 슬립 기능을 제공하는 프로그램입니다.
// alarm 시그널을 사용하여 주어진 시간만큼 프로그램을 일시 중지하고,
// SIGALRM 시그널을 받으면 프로그램이 다시 실행되도록 합니다.
// 이를 통해 사용자 정의 슬립 함수를 구현하고 5초 동안 대기한 후에 깨어납니다.