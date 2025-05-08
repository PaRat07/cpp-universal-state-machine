#pragma once

#include <variant>
#include <chrono>
#include <queue>


#include "resume_conditions.h"

template <typename... TaskTs>
class TimedEventLoop {
 public:
    using time_t = std::chrono::steady_clock::time_point;

    struct TaskHolder {
      time_t wait_for_time;
      std::variant<TaskTs...> task;

      constexpr auto operator<=>(const TaskHolder &rhs) const {
          return wait_for_time <=> rhs.wait_for_time;
      }
    };

    template<typename T>
    void AddTask(WaitUntil<T>&& task)
            requires (std::is_same_v<T, TaskTs> || ...) {
        tasks_.emplace(task.cond, std::move(task.to_resume));
    }

    template<typename T>
    void AddTask(WaitFor<T>&& task)
            requires (std::is_same_v<T, TaskTs> || ...) {
        tasks_.emplace(std::chrono::steady_clock::now() + task.cond, std::move(task.to_resume));
    }

    void Resume(auto &&st_mach) {
        while (!tasks_.empty() && tasks_.top().wait_for_time < std::chrono::steady_clock::now()) {
            auto [_, task] = std::move(tasks_.top());
            tasks_.pop();
            task.visit([&st_mach] (auto &&task) {
                if constexpr (std::is_same_v<decltype(task.Resume()), void>) {
                    task.Resume();
                } else {
                    st_mach.AddTask(task.Resume());
                }
            });
        }
    }

 private:
    std::priority_queue<TaskHolder> tasks_;
};
