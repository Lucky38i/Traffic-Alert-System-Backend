cmake_minimum_required(VERSION 3.15)
project(TAS_Backend)

message(STATUS "start running cmake...")

set(CMAKE_CXX_STANDARD 11)
include_directories(. src)
add_executable(server src/server.cpp)