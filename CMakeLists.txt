cmake_minimum_required(VERSION 3.28.3)
project(learning)
find_package(Crow REQUIRED)
find_package(Threads)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(THREADS_PREFER_PTHREAD_FLAG ON)

include_directories(${Crow_INCLUDE_DIRS} ${Threads_INCLUDE_DIRS})
link_directories(${Crow_DIRS} ${Threads_DIRS})

add_subdirectory(hello_crow)