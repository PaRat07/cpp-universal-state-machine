cmake_minimum_required(VERSION 3.28)
project(example)

set(CMAKE_CXX_STANDARD 26)

add_executable(${PROJECT_NAME} main.cpp test_task.cpp)

target_link_libraries(${PROJECT_NAME} universal-event-loop)
