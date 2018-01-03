cmake_minimum_required(VERSION 3.9)
project(edet)

set(CMAKE_CXX_STANDARD 17)

add_executable(edet main.cpp)
target_link_libraries(edet cal)
target_include_directories(edet PUBLIC ${CMAKE_SOURCE_DIR}/libcal/include)
