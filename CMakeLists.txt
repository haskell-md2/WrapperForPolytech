cmake_minimum_required(VERSION 2.8.2)

project(wrapper_using_example)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(PROG_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/include")
include_directories(${PROG_INCLUDE})

add_executable(WrapperExample example/main.cpp)

set_target_properties(WrapperExample PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)