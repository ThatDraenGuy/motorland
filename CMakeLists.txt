cmake_minimum_required(VERSION 3.11)

project(motorland C)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(motorsd)
#add_subdirectory(state_manager)
#add_subdirectory(config_manager)
#add_subdirectory(proto_libs)
