cmake_minimum_required(VERSION 3.10)

project(atmos-line)

find_package(Boost COMPONENTS program_options REQUIRED)

include_directories("${CMAKE_SOURCE_DIR}/src")

file(GLOB_RECURSE SRC "*.cpp")
add_executable(${PROJECT_NAME} ${SRC})

target_link_libraries(${PROJECT_NAME} PRIVATE atmos)
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

install(TARGETS ${PROJECT_NAME})
