#include <chrono>
#include <type_traits>


#include "ev-loop/state_machine.h"
#include "ev-loop/timed_event_loop.h"
#include "ev-loop/io_uring_event_loop.h"


#include "test_timed_task.h"
#include "test_io_task.h"



using namespace std::string_view_literals;
using namespace std::string_literals;

using TasksTs = std::type_identity<std::tuple<
    TimedTask<TimedTaskState::kStarted>,
    TimedTask<TimedTaskState::kWaited500>,
    TimedTask<TimedTaskState::kWaited1000>,
    TimedTask<TimedTaskState::kWaited1500>,

    TestIOTask<IOTaskState::kStarted>,
    TestIOTask<IOTaskState::kWritedGoyda>,
    TestIOTask<IOTaskState::kWritedZov>,
    TestIOTask<IOTaskState::kReaded>
>>;


#include "fcntl.h"

int main() {
    [] <typename... Tasks> (std::type_identity<std::tuple<Tasks...>>) {
        using StMach = StateMachine<TimedEventLoop<Tasks...>, IOUringEventLoop<Tasks...>>;
        StMach st_mach;
        st_mach.AddTask(WaitUntil(std::chrono::steady_clock::now(), TimedTask<TimedTaskState::kStarted>{}));
        st_mach.AddTask(WaitUntil(std::chrono::steady_clock::now(), TimedTask<TimedTaskState::kStarted>{}));
        int fd = open("/tmp/test.txt", O_RDWR | O_CREAT | O_TRUNC);
        st_mach.AddTask(WaitUntil(std::chrono::steady_clock::now(), TestIOTask<IOTaskState::kStarted>{ fd }));
        st_mach.RunLoop();
    } (TasksTs{});
}
