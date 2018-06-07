#include "threaded_queue.h"

// static
bool MultiQueueWaiter::HasState(
    std::initializer_list<BaseThreadQueue*> queues) {
  for (BaseThreadQueue* queue : queues) {
    if (!queue->IsEmpty())
      return true;
  }
  return false;
}

bool MultiQueueWaiter::ValidateWaiter(
    std::initializer_list<BaseThreadQueue*> queues) {
  for (BaseThreadQueue* queue : queues) {
    if (queue->waiter.get() != this)
      return false;
  }
  return true;
}
