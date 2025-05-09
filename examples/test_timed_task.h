#pragma once

#include "ev-loop/state_machine.h"

#include <ev-loop/resume_conditions.h>
#include <chrono>
#include <print>

enum class TimedTaskState {
    kStarted,
    kWaited500,
    kWaited1000,
    kWaited1500
};

template<TimedTaskState>
struct TimedTask;

template<>
struct TimedTask<TimedTaskState::kStarted> {
  WaitFor<TimedTask<TimedTaskState::kWaited500>> Resume();
  int num;
};

template<>
struct TimedTask<TimedTaskState::kWaited500> {
  WaitFor<TimedTask<TimedTaskState::kWaited1000>> Resume();
  int num;
};

template<>
struct TimedTask<TimedTaskState::kWaited1000> {
  WaitFor<TimedTask<TimedTaskState::kWaited1500>> Resume();
  int num;
};


template<>
struct TimedTask<TimedTaskState::kWaited1500> {
  void Resume();
  int num;
};
