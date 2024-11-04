# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目的名称是 Demo 
project (Demo)

add_definitions(-std=c++11)

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++11 ")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

# 将源文件main.cpp编译成一个名称为 Demo 的可执行文件
add_executable(Demo main.cpp)
