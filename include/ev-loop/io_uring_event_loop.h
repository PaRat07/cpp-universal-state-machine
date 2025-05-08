#pragma once

#include <chrono>
#include <queue>
#include <span>
#include <utility>
#include <variant>
#include <ranges>

#include <liburing/io_uring.h>
#include <unistd.h>

// #include "dynamic_variant.h"
#include "resume_conditions.h"

#include <liburing.h>

template <typename... TaskTs>
class IOUringEventLoop {
 public:
  using UniversalTaskT = std::variant<TaskTs...>;

  static constexpr size_t kBufSz = 4096;

  IOUringEventLoop() {
    {
        void* buffer;
        if (posix_memalign(&buffer, kBufSz, kBufSz) != 0) {
        throw std::runtime_error("posix_memalign failed");
        }
        buf = std::span<std::byte, kBufSz>(reinterpret_cast<std::byte*>(buffer), kBufSz);
    }

    if (io_uring_queue_init(8, &ring, 0) < 0) {
      free(buf.data());
      throw std::runtime_error("io_uring_queue_init failed");
    }
  }


  template<typename T>
  void AddTask(AsyncRead<T>&& task)
          requires (std::is_same_v<T, TaskTs> || ...) {
    io_uring_sqe* sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, task.fd, task.cond.data.data(), task.cond.data.size(), 0);
    sqe->user_data = new UniversalTaskT(std::in_place_type<T>, std::move(task.to_resume));
    io_uring_submit(&ring);
  }

  void Resume(auto &&st_mach) {
      static constexpr size_t kMaxPeek = 5;
      std::array<io_uring_cqe*, kMaxPeek> cqes;
      int ready_cnt = io_uring_peek_batch_cqe(&ring, cqes.data(), kMaxPeek);
      if (ready_cnt < 0) {
        throw std::runtime_error("io_uring_peek_batch_cqe failed");
      }
      for (io_uring_cqe *copl : cqes | std::views::take(ready_cnt)) {
        reinterpret_cast<UniversalTaskT>(copl->user_data).visit([&st_mach] (auto &task) {
          if constexpr (std::is_same_v<decltype(task.Resume()), void>) {
              task.Resume();
          } else {
              st_mach.AddTask(task.Resume());
          }
        });
      }
  }

  ~IOUringEventLoop() {
      std::free(buf.data());
  }

 private:
  io_uring ring;
  std::span<std::byte, kBufSz> buf;
};
