
#include <cstdint>
#include <cstdio>

#include "app_layer_v1/byte_queue.h"

const int BUF_SIZE = 10;

int main() {
  printf("test\n");
  uint8_t buffer[BUF_SIZE];
  ByteQueue queue;
  ByteQueueInit(&queue, buffer, BUF_SIZE);
  for (int i = 1; i < BUF_SIZE; i++) {
    ByteQueuePush(&queue, i);
  }
  int i = BUF_SIZE;
  while (ByteQueueSize(&queue) > 0) {
    uint8_t b = ByteQueuePop(&queue);
    printf("b=%d\n", b);
  }
  uint8_t extra[BUF_SIZE] = {11,22,33,44,55,66,77,88,99,101};
  ByteQueuePushBuffer(&queue, extra, BUF_SIZE);
  printf("size=%d\n", ByteQueueSize(&queue));
  while (ByteQueueSize(&queue) > 0) {
    uint8_t b = ByteQueuePop(&queue);
    printf("b=%d\n", b);
  }
  
  return 0;
}
