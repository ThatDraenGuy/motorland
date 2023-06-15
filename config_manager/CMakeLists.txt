cmake_minimum_required(VERSION 3.11)

project(config_manager C)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
include_directories(include)

include(lib_yaml_install.cmake)
include(lib_cyaml_install.cmake)

add_executable(${PROJECT_NAME} main.c)

target_sources(${PROJECT_NAME}
        PRIVATE
        ${SOURCES}
        PUBLIC
        ${HEADERS}
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        PRIVATE ${LIBYAML_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME} PRIVATE libcyaml libyaml gpio_lib_lib)

# library
add_library(${PROJECT_NAME}_lib ${SOURCES})

target_link_libraries(${PROJECT_NAME}_lib PUBLIC libcyaml libyaml gpio_lib_lib)
target_include_directories(${PROJECT_NAME}_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include ${LIBCYAML_INCLUDE_DIRS})