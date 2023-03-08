cmake_minimum_required(VERSION 3.22)
project(thrust_test)

include(cmake/configure.cmake)
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

include_directories(src)
add_subdirectory(src)
