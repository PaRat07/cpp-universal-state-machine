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
  template<typename T>
  struct CallbackData {
    ssize_t T::* res_ptr;
    T value;
  };

  using UniversalTaskT = std::variant<CallbackData<TaskTs>..., TaskTs...>;



  IOUringEventLoop() {
    if (io_uring_queue_init(8, &ring, 0) < 0) {
      throw std::runtime_error("io_uring_queue_init failed");
    }
  }

  template<typename T>
  void AddTask(AsyncRead<T>&& task)
          requires (std::is_same_v<T, TaskTs> || ...) {
    io_uring_sqe* sqe = io_uring_get_sqe(&ring);
    io_uring_prep_read(sqe, task.cond.fd, task.cond.data.data(), task.cond.data.size(), 0);
    sqe->user_data = std::bit_cast<__u64>(new UniversalTaskT(std::in_place_type<CallbackData<T>>, task.cond.res_ptr, std::move(task.to_resume)));
    io_uring_submit(&ring);
  }

  template<typename T>
  void AddTask(AsyncWrite<T>&& task)
          requires (std::is_same_v<T, TaskTs> || ...) {
    io_uring_sqe* sqe = io_uring_get_sqe(&ring);
    io_uring_prep_write(sqe, task.cond.fd, task.cond.data.data(), task.cond.data.size(), -1);
    sqe->user_data = std::bit_cast<__u64>(new UniversalTaskT(std::in_place_type<T>, std::move(task.to_resume)));
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
        std::bit_cast<UniversalTaskT*>(copl->user_data)->visit([&st_mach, res = copl->res] (auto &task) {
          if constexpr ((!std::same_as<std::remove_cvref_t<decltype(task)>, TaskTs> && ...)) {
            std::invoke(task.res_ptr, task.value) = res;
            if constexpr (std::is_same_v<decltype(task.value.Resume()), void>) {
                task.value.Resume();
            } else {
                st_mach.AddTask(task.value.Resume());
            }
          } else {
            if constexpr (std::is_same_v<decltype(task.Resume()), void>) {
                task.Resume();
            } else {
                st_mach.AddTask(task.Resume());
            }
          }
        });
        delete reinterpret_cast<UniversalTaskT*>(copl->user_data);
        io_uring_cqe_seen(&ring, copl);
      }
  }

  ~IOUringEventLoop() {
  }

 private:
  io_uring ring;
};
