
#include <cstdint>
#include <cstdio>
#include <cassert>

#include "app_layer_v1/byte_queue.h"
#include "app_layer_v1/protocol.h"

void testByteQueue() {
  const int BUF_SIZE = 10;
  int index = 1;
  int size = 0;
  int size2 = 0;
  const uint8_t *data = nullptr;
  const uint8_t *data2 = nullptr;

  uint8_t buffer[BUF_SIZE];
  ByteQueue queue;

  ByteQueueInit(&queue, buffer, BUF_SIZE);

  for (int fill = 2; fill < BUF_SIZE; fill++) {
    for (int i = 0; i < fill; i++) {
      ByteQueuePush(&queue, i);
    }
    ByteQueuePeek(&queue, &data, &size);
    fprintf(stderr, "fill:%d size:%d\n", fill, size);
    assert(fill == size);
    ByteQueueDiscard(&queue, fill);
  }
  
  while (ByteQueueSize(&queue) > 0) {
    uint8_t popped = ByteQueuePop(&queue);
    uint8_t expected = index++;
    assert(popped == expected);
  }

  uint8_t pushedBuffer[BUF_SIZE] = {11,22,33,44,55,66,77,88,99,101};
  ByteQueuePushBuffer(&queue, pushedBuffer, BUF_SIZE);
  assert(BUF_SIZE == ByteQueueSize(&queue));

  ByteQueuePeekMax(&queue, BUF_SIZE, &data, &size, &data2, &size2);
  // not sure if this makes sense
  assert(11 == *data);
  //assert(22 == *data2);
  //assert(1 == size);
  //assert(9 == size2);
  
  index = 0;
  while (ByteQueueSize(&queue) > 0) {
    uint8_t popped = ByteQueuePop(&queue);
    uint8_t expected = pushedBuffer[index++];
    assert(popped == expected);
  }

  ByteQueuePush(&queue, 80);
  ByteQueuePush(&queue, 81);
  ByteQueuePush(&queue, 82);
  ByteQueuePeek(&queue, &data, &size);
  assert(size == 3);
  assert(80 == *data);
  assert(81 == *(data+1));
  assert(82 == *(data+2));
  ByteQueuePop(&queue); // pop 80
  
  ByteQueuePeek(&queue, &data, &size);
  assert(size == 2);
  assert(81 == *data);
  assert(82 == *(data+1));
  ByteQueuePop(&queue); // pop 81
  
  ByteQueuePeek(&queue, &data, &size);
  assert(size == 1);
  assert(82 == *data);
  ByteQueuePop(&queue); // pop 82

  ByteQueuePush(&queue, 83);
  ByteQueuePeek(&queue, &data, &size);
  assert(size == 1);
  assert(83 == *data);
  
  ByteQueueDiscard(&queue, 1);
  ByteQueuePeek(&queue, &data, &size);
  assert(size == 0);

  ByteQueuePushBuffer(&queue, pushedBuffer, BUF_SIZE);
  index = 0;
  while (ByteQueueSize(&queue) > 0) {
    uint8_t popped = ByteQueuePop(&queue);
    uint8_t expected = pushedBuffer[index++];
    assert(popped == expected);
  }
  
}

void testProtocol() {
  AppProtocolInit(1);
  AppProtocolTasks(1);
}

int main() {
  testByteQueue();
  testProtocol();
  return 0;
}
