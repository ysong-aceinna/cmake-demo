cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo17)

# set (CMAKE_INCLUDE_CURRENT_DIR ON)

set(COMPILE_TYPE Debug)
if(COMPILE_TYPE STREQUAL "Release")
  set(CMAKE_BUILD_TYPE "Release")
  set(CMAKE_CXX_FLAGS "-std=c++11 ")
  set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall")
elseif(COMPILE_TYPE STREQUAL "Debug")
  SET(CMAKE_BUILD_TYPE "Debug")
  set(CMAKE_CXX_FLAGS "-std=c++11 ")
  SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
else()
  message(FATAL_ERROR "COMPILE_TYPE is Error!")
endif()
  message(AUTHOR_WARNING "COMPILE_TYPE = ${COMPILE_TYPE}")

find_package(Eigen3 REQUIRED)

include_directories(${CERES_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIR})

# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

# 指定生成目标
add_executable(Demo ${DIR_SRCS})
target_link_libraries (Demo  ${OpenCV_LIBS} ${CERES_LIBRARIES})

