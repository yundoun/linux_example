#include <stdio.h>
#include <stdlib.h>

// 데이터 구조 정의
typedef struct
{
  int *data; // 동적으로 할당된 데이터 배열
  int size;  // 배열 크기
} DynamicData;

// 파괴자 함수: 동적으로 할당된 데이터를 해제
void destroyDynamicData(DynamicData *dd)
{
  if (dd->data != NULL)
  {
    free(dd->data); // 동적으로 할당된 데이터 해제
    dd->data = NULL;
    dd->size = 0;
  }
}

int main()
{
  // 동적 데이터 생성
  DynamicData myData;
  myData.size = 5;
  myData.data = (int *)malloc(sizeof(int) * myData.size);

  if (myData.data == NULL)
  {
    perror("메모리 할당 실패");
    return 1;
  }

  // 데이터 초기화
  for (int i = 0; i < myData.size; i++)
  {
    myData.data[i] = i * 2;
  }

  // 데이터 출력
  printf("동적으로 생성된 데이터:\n");
  for (int i = 0; i < myData.size; i++)
  {
    printf("%d ", myData.data[i]);
  }
  printf("\n");

  // 파괴자 함수 호출하여 메모리 해제
  destroyDynamicData(&myData);

  // 데이터 해제 후 확인
  if (myData.data == NULL)
  {
    printf("메모리 해제 성공\n");
  }
  else
  {
    printf("메모리 해제 실패\n");
  }

  return 0;
}

// 이 코드를 실행하면 동적으로 할당된 데이터를 초기화하고 출력한 후,
// 메모리를 해제하여 메모리 누수를 방지하는 방법을 볼 수 있습니다.