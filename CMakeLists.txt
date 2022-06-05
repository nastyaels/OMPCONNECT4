cmake_minimum_required(VERSION 3.22)
project(OMPCONNECT4)

set(CMAKE_CXX_STANDARD 14)

add_executable(OMPCONNECT4 main.cpp)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(OMPCONNECT4 PUBLIC OpenMP::OpenMP_CXX)
endif()