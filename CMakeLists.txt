cmake_minimum_required(VERSION 3.22)
project(settingpp)

set(CMAKE_CXX_STANDARD 17)
include_directories(src)
include_directories(include)
add_executable(settingpp tests/main_test.cpp)
