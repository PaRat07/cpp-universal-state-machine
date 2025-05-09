#pragma once

#include <chrono>
#include <span>

template<typename T>
struct WaitFor {
    std::chrono::steady_clock::duration cond;
    T to_resume;
};

template<typename T>
struct WaitUntil {
    std::chrono::steady_clock::time_point cond;
    T to_resume;
};

template<typename T>
struct AsyncRead {
    struct {
        int fd;
        std::span<char8_t> data;
        ssize_t T::* res_ptr;
    } cond;
    T to_resume;
};

template<typename T>
struct AsyncWrite {
    struct {
        int fd;
        std::span<const char8_t> data;
    } cond;
    T to_resume;
};
