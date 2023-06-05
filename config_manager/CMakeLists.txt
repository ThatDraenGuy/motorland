cmake_minimum_required(VERSION 3.11)

project(config_manager C)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
include_directories(include)

find_library(LIBCYAML_LIBRARY cyaml)
include_directories(/usr/local/include)

add_executable(${PROJECT_NAME} src/config_parser.c)
target_link_libraries(${PROJECT_NAME} ${LIBCYAML_LIBRARY})