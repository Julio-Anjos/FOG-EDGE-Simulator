cmake_minimum_required(VERSION 2.8.8)

project(FogSimulator) 

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")



include_directories(${SimGrid_INCLUDE_DIR})

include_directories(
        ${PROJECT_SOURCE_DIR}/src
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/cparse
)

file(GLOB all_SRCS
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
        "${PROJECT_SOURCE_DIR}/src/*.c"
        
        )
file(GLOB cparse_binaries
		"${PROJECT_SOURCE_DIR}/cparse/core-shunting-yard.o"
        "${PROJECT_SOURCE_DIR}/cparse/builtin-features.o"

		)

add_executable(main ${all_SRCS} ${cparse_binaries})
target_link_libraries(main ${SimGrid_LIBRARY})
