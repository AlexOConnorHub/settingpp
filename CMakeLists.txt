cmake_minimum_required(VERSION 3.12)
project("settingpp" VERSION 1.0.0 DESCRIPTION "A header only c++ library that provides settings and configuration management")
set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(src)
add_library(src INTERFACE)

file(GLOB_RECURSE SRCS tests/*.cpp)
add_executable(settingpp ${SRCS})
