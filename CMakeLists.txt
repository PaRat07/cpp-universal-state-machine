cmake_minimum_required(VERSION 3.28)

project(statemachine)

set(CMAKE_CXX_STANDARD 26)

add_library(${PROJECT_NAME} STATIC "src/noop.cpp")

target_include_directories(${PROJECT_NAME}
    PUBLIC "${CMAKE_SOURCE_DIR}/include")

add_subdirectory(examples)
