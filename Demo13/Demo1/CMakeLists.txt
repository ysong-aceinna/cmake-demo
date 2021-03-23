cmake_minimum_required(VERSION 2.8)
project(jsoncpp_test1)

set(CMAKE_CXX_FLAGS "-std=c++11 ")
include_directories ("${PROJECT_SOURCE_DIR}")

set(JSONCPP_LIBS ${PROJECT_SOURCE_DIR}/lib/libjsoncpp.1.9.4.dylib) 

add_executable(jsoncpp_test1 main.cpp)

target_link_libraries(jsoncpp_test1 ${JSONCPP_LIBS})
