cmake_minimum_required(VERSION 3.9)
project(tpx2h5)

set(CMAKE_CXX_STANDARD 17)

add_executable(tpx2h5 main.cpp)
target_link_libraries(tpx2h5 hdf5 hdf5_cpp)
