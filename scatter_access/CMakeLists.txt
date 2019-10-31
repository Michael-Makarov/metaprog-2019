cmake_minimum_required(VERSION 3.12)
project(scatter_access)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --std=c++03")

add_executable(scatter_access main.cpp)