add_library(common motor_internal.h motor_internal.c)

target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})