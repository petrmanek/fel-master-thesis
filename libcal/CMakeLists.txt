cmake_minimum_required(VERSION 3.9)
project(libcal)

set(CMAKE_CXX_STANDARD 17)

file(GLOB LIBCAL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB LIBCAL_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

add_library(cal SHARED ${LIBCAL_SOURCES} ${LIBCAL_HEADERS})
include_directories(cal ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(cal hdf5 hdf5_cpp)
