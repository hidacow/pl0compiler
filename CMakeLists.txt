cmake_minimum_required(VERSION 3.24)
project(pl0c)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
add_executable(${PROJECT_NAME}
        main.cpp)
