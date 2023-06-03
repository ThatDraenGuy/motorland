cmake_minimum_required(VERSION 3.11)
project(proto_libs C)

include_directories(include)

add_subdirectory(common)
add_subdirectory(gpio_lib)
add_subdirectory(camhi_uart_lib)