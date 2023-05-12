cmake_minimum_required(VERSION 3.11)

project(motorland C)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set compiler flags (optional)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(daemon)
add_subdirectory(state_manager)
add_subdirectory(config_manager)
add_subdirectory(proto_libs)
