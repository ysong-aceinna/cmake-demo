cmake_minimum_required(VERSION 2.8)
project(jsoncpp_test2)

set(CMAKE_CXX_FLAGS "-std=c++11 ")
include_directories ("${PROJECT_SOURCE_DIR}/jsoncpp")

add_executable(jsoncpp_test2 main.cpp ${PROJECT_SOURCE_DIR}/jsoncpp/jsoncpp.cpp)
