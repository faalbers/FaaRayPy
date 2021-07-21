cmake_minimum_required(VERSION 3.4)
project(FaaRayPy)
#enable_testing()

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(extern/pybind11)
add_subdirectory(extern/FaaRay)

execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
