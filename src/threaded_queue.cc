#include "threaded_queue.h"

namespace {
struct DummyLockable {
  void lock() {}
  void unlock() {}
};
}

void MultiQueueWaiter::Wait(std::initializer_list<BaseThreadQueue*> queues) {
  cv.wait(DummyLockable(), [&]() {
    for (const auto& queue : queues) {
      if (!queue->IsEmpty())
        return true;
    }
    return false;
  });
}