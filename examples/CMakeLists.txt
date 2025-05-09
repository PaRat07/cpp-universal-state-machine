cmake_minimum_required(VERSION 3.28)
project(example)

set(CMAKE_CXX_STANDARD 26)

add_executable(${PROJECT_NAME} main.cpp test_timed_task.cpp test_io_task.cpp)

target_link_options(${PROJECT_NAME} PUBLIC -luring)

target_link_libraries(${PROJECT_NAME} universal-event-loop)
