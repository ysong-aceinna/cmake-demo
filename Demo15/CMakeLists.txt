cmake_minimum_required(VERSION 3.2)

project(yaml_test)

add_definitions(-std=c++11)

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++11 ")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

include_directories(include)
set(SRCS yaml_test.cpp)
add_executable(yamltest ${SRCS})

target_link_libraries(yamltest ${CMAKE_HOME_DIRECTORY}/lib/libyaml-cpp.a)

