cmake_minimum_required(VERSION 3.9)
project(gen)

set(CMAKE_CXX_STANDARD 17)

add_executable(gen main.cpp)
target_link_libraries(gen cal)
target_include_directories(gen PUBLIC ${CMAKE_SOURCE_DIR}/libcal/include)
