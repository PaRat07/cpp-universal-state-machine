#include "test_timed_task.h"

namespace chr = std::chrono;

WaitFor<TimedTask<TimedTaskState::kWaited500>>
TimedTask<TimedTaskState::kStarted>::Resume() {
  std::println("kStarted: {}", num);
  return {
      chr::milliseconds(500),
      {13}
  };
}
WaitFor<TimedTask<TimedTaskState::kWaited1000>>
TimedTask<TimedTaskState::kWaited500>::Resume() {
  std::println("kWaited500: {}", num);
  return {
      std::chrono::milliseconds(500),
      {13}
  };
}

WaitFor<TimedTask<TimedTaskState::kWaited1500>>
TimedTask<TimedTaskState::kWaited1000>::Resume() {
  std::println("kWaited1000: {}", num);
  return {
      std::chrono::milliseconds(500),
      {52}
  };
}

void TimedTask<TimedTaskState::kWaited1500>::Resume() {
  std::println("kWaited1500: {}", num);
}
