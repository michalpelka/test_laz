cmake_minimum_required(VERSION 3.17)
project(test_las)

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(3rd/LASZip/)

include_directories(3rd/LASZip/include)

add_executable(test_las main.cpp)
target_link_libraries(test_las laszip3)