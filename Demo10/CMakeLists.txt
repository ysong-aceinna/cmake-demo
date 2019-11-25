cmake_minimum_required(VERSION 2.8)
project(ceres)

find_package(Ceres REQUIRED)
include_directories(${CERES_INCLUDE_DIRS})

add_executable(use_ceres1 main1.cpp)
target_link_libraries(use_ceres1 ${CERES_LIBRARIES})

# add_executable(use_ceres2 main2.cpp)
# target_link_libraries(use_ceres2 ${CERES_LIBRARIES})

add_executable(use_ceres3 main3.cpp)
target_link_libraries(use_ceres3 ${CERES_LIBRARIES})

add_executable(use_ceres4 main4.cpp)
target_link_libraries(use_ceres4 ${CERES_LIBRARIES})
