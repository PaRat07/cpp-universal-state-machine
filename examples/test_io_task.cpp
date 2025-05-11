#include "test_io_task.h"

#include "unistd.h"

#include <iomanip>

AsyncWrite<TestIOTask<IOTaskState::kWritedZov>>
TestIOTask<IOTaskState::kStarted>::Resume() {
  std::println("Began writing zov");
  std::u8string_view data = u8"goyda";
  return {
    .cond = {
      .fd = fd,
      .data = data
    },
    .to_resume = {
      .fd = fd,
      .to_write = u8"zov"
    }
  };
}
AsyncWrite<TestIOTask<IOTaskState::kWritedGoyda>>
TestIOTask<IOTaskState::kWritedZov>::Resume() {
  std::println("Writed zov");
  std::println("Began writing goyda");
  return {
    .cond = {
      .fd = fd,
      .data = to_write
    },
    .to_resume = { .fd = fd }
  };
}
AsyncRead<TestIOTask<IOTaskState::kReaded>>
TestIOTask<IOTaskState::kWritedGoyda>::Resume() {
  std::println("Writed goyda");
  std::println("Began reading");
  auto data = std::make_unique_for_overwrite<char8_t[]>(8);
  std::fill_n(data.get(), 8, u8'p');
  auto data_ptr = data.get();
  return {
    .cond = {
      .fd = fd,
      .data = {data_ptr, 8},
      .res_ptr = &TestIOTask<IOTaskState::kReaded>::readed_cnt
    },
    .to_resume = {
      .data = std::move(data),
      .fd = fd
    }
  };
}
void TestIOTask<IOTaskState::kReaded>::Resume() {
  std::println("Readed: \"{}\"",
    std::string_view((char*)data.get(), 8));
}