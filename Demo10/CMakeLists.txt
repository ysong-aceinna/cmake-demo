cmake_minimum_required(VERSION 2.8)
project(ceres)

SET(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_CXX_FLAGS "-std=c++11 ")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

add_executable(ceres1 main1.cpp)
target_link_libraries(ceres1 ${CERES_LIBRARIES})

find_package(OpenCV REQUIRED)
add_executable(ceres2 main2.cpp)
target_link_libraries(ceres2 ${CERES_LIBRARIES} ${OpenCV_LIBS})

add_executable(ceres3 main3.cpp)
target_link_libraries(ceres3 ${CERES_LIBRARIES})

add_executable(ceres4 main4.cpp)
target_link_libraries(ceres4 ${CERES_LIBRARIES})

add_executable(ceres5 main5.cpp)
target_link_libraries(ceres5 ${CERES_LIBRARIES})
