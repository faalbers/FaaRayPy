cmake_minimum_required(VERSION 3.4)
project(FaaRayPy)
#enable_testing()

set(CMAKE_CXX_STANDARD 11)

#add_compile_definitions(CLASS_DEBUG)
#add_compile_definitions(SHARED_POINTER_DEBUG)

execute_process(COMMAND git submodule update --init --recursive
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

find_package (Python3 COMPONENTS Interpreter Development)
add_subdirectory(extern/pybind11)
add_subdirectory(extern/FaaRay)
add_subdirectory(faaray)
