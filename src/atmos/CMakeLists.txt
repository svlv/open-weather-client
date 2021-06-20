cmake_minimum_required(VERSION 3.10)

project(atmos)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

#include_directories("src")

file(GLOB_RECURSE SRC "*.cpp")
#file(GLOB_RECURSE INC "*.hpp")
add_library(${PROJECT_NAME} SHARED ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
