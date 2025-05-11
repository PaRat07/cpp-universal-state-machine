#pragma once

#include <tuple>


template <typename... EventLoopsTs>
class StateMachine {
 public:
  void AddTask(auto &&task) {
      std::apply([&task] (auto&&... event_loops) mutable {
          static_assert((requires { event_loops.AddTask(std::move(task)); } + ...) == 1);
          ([&task] (auto &&event_loop) mutable {
              if constexpr (requires { event_loop.AddTask(std::move(task)); }) {
                  event_loop.AddTask(std::move(task));
              }
          } (event_loops), ...);
      }, event_loops_);
  }


  void RunLoop() {
      while (std::apply([] (auto&&... loops) { return (!loops.Empty() || ...); }, event_loops_)) {
          std::apply([this] (auto&&... event_loops) mutable {
              ([this] (auto &&event_loop) mutable {
                  event_loop.Resume(*this);
              } (event_loops), ...);
          }, event_loops_);
      }
  }

 private:
  std::tuple<EventLoopsTs...> event_loops_;
};
