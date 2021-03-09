cmake_minimum_required(VERSION 2.8)
project(test)

set(MATLAB_PATH /Applications/MATLAB_R2018a.app)

INCLUDE_DIRECTORIES(
    ${MATLAB_PATH}/extern/include
    )

set(MATLAB_LIBS 
    ${MATLAB_PATH}/bin/maci64/libeng.dylib
    ${MATLAB_PATH}/bin/maci64/libmx.dylib
    ${MATLAB_PATH}/bin/maci64/libmex.dylib
    ${MATLAB_PATH}/bin/maci64/libmat.dylib
) 

add_executable(test main2.cpp)

target_link_libraries(test
                      ${MATLAB_LIBS}
)
