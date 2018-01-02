cmake_minimum_required(VERSION 3.9)
project(h52tpx)

set(CMAKE_CXX_STANDARD 17)

add_executable(h52tpx main.cpp)
target_link_libraries(h52tpx cal)
target_include_directories(h52tpx PUBLIC ${CMAKE_SOURCE_DIR}/libcal/include)
