#pragma once

#include "ev-loop/state_machine.h"


#include <chrono>
#include <print>



enum class TestTaskState {
    kStarted,
    kWaited500,
    kWaited1000,
    kWaited1500
};

template<TestTaskState>
struct TesTask;

template<typename T>
using TaskHolder = std::pair<std::chrono::steady_clock::time_point, T>;

template<>
struct TesTask<TestTaskState::kStarted> {
  TaskHolder<TesTask<TestTaskState::kWaited500>> Resume();
  int num;
};

template<>
struct TesTask<TestTaskState::kWaited500> {
  TaskHolder<TesTask<TestTaskState::kWaited1000>> Resume();
  int num;
};

template<>
struct TesTask<TestTaskState::kWaited1000> {
  TaskHolder<TesTask<TestTaskState::kWaited1500>> Resume();
  int num;
};


template<>
struct TesTask<TestTaskState::kWaited1500> {
  void Resume();
  int num;
};
