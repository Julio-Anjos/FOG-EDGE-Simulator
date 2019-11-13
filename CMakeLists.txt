cmake_minimum_required(VERSION 2.8.8)

project(FogSimulator) # TODO: give a real name to your project here

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(SimGrid 3.22 REQUIRED) # This template requires SimGrid v3.22

include_directories(${SimGrid_INCLUDE_DIR})

add_executable(main main.cpp)
target_link_libraries(main ${SimGrid_LIBRARY})

