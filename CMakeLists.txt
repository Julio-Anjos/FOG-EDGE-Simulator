cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(FogSimulator) 

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

find_package(SimGrid 3.24 REQUIRED) # This template requires SimGrid v3.24

include_directories(
	${SimGrid_INCLUDE_DIR}
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
