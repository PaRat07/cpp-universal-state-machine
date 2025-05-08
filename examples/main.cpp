#include <chrono>
#include <type_traits>
#include "ev-loop/state_machine.h"
#include "ev-loop/timed_event_loop.h"
#include "test_task.h"

#include "ev-loop/io_uring_event_loop.h"


using namespace std::string_view_literals;
using namespace std::string_literals;

using TasksTs = std::type_identity<std::tuple<
    TesTask<TestTaskState::kStarted>,
    TesTask<TestTaskState::kWaited500>,
    TesTask<TestTaskState::kWaited1000>,
    TesTask<TestTaskState::kWaited1500>
>>;

int main() {
    [] <typename... Tasks> (std::type_identity<std::tuple<Tasks...>>) {
        using StMach = StateMachine<TimedEventLoop<Tasks...>>;
        StMach st_mach;
        st_mach.AddTask(std::pair(std::chrono::steady_clock::now(), TesTask<TestTaskState::kStarted>{}));
        st_mach.RunLoop();
    } (TasksTs{});
}
