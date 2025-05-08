#include "test_task.h"
#include <chrono>

TaskHolder<TesTask<TestTaskState::kWaited500>>
TesTask<TestTaskState::kStarted>::Resume() {
  std::println("kStarted: {}", num);
  return {
      std::chrono::steady_clock::now() + std::chrono::milliseconds(500),
      {13}
  };
}
TaskHolder<TesTask<TestTaskState::kWaited1000>>
TesTask<TestTaskState::kWaited500>::Resume() {
  std::println("kWaited500: {}", num);
  return {
      std::chrono::steady_clock::now() + std::chrono::milliseconds(500),
      {13}
  };
}

TaskHolder<TesTask<TestTaskState::kWaited1500>>
TesTask<TestTaskState::kWaited1000>::Resume() {
  std::println("kWaited1000: {}", num);
  return {
      std::chrono::steady_clock::now() + std::chrono::milliseconds(500),
      {52}
  };
}

void TesTask<TestTaskState::kWaited1500>::Resume() {
  std::println("kWaited1500: {}", num);
}
