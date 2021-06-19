cmake_minimum_required(VERSION 3.10)

project(open-weather-client)

set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

include_directories("src")

file(GLOB_RECURSE SRC "src/*.cpp")
file(GLOB_RECURSE INC "src/*.hpp")
add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)
