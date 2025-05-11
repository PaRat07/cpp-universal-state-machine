#pragma once

#include "ev-loop/state_machine.h"

#include "ev-loop/resume_conditions.h"
#include <chrono>
#include <print>

enum class IOTaskState {
  kStarted,
  kWritedZov,
  kWritedGoyda,
  kReaded
};

template<IOTaskState>
struct TestIOTask;

template<>
struct TestIOTask<IOTaskState::kStarted> {
  AsyncWrite<TestIOTask<IOTaskState::kWritedZov>> Resume();
  int fd;
};

template<>
struct TestIOTask<IOTaskState::kWritedZov> {
  AsyncWrite<TestIOTask<IOTaskState::kWritedGoyda>> Resume();
  int fd;
  std::u8string_view to_write;
};


template<>
struct TestIOTask<IOTaskState::kWritedGoyda> {
  AsyncRead<TestIOTask<IOTaskState::kReaded>> Resume();
  int fd;
};


template<>
struct TestIOTask<IOTaskState::kReaded> {
  void Resume();
  std::unique_ptr<char8_t[]> data;
  int fd;
  ssize_t readed_cnt;
};