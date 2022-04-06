cmake_minimum_required(VERSION 3.10)

project(atmos)
add_compile_definitions(BOOST_BEAST_USE_STD_STRING_VIEW)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

file(GLOB_RECURSE SRC "*.cpp")
add_library(${PROJECT_NAME} SHARED ${SRC})

find_package(Boost REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PRIVATE Threads::Threads)

install(TARGETS ${PROJECT_NAME})
