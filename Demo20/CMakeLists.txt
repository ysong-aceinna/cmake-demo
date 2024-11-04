
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo)

# 查找目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
aux_source_directory(. DIR_SRCS)

SET(CMAKE_BUILD_TYPE "Debug")
# set(CMAKE_CXX_FLAGS "-std=c++11 ")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

# 指定生成目标
add_executable(Demo ${DIR_SRCS})
