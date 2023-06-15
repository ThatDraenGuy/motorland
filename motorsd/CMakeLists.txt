project(motorsd C)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
include_directories(include)

#set(CONF_LIB_ROOT ${CMAKE_SOURCE_DIR}/config_manager)
#set(CONF_LIB_DIR ${LIBGPIOD_ROOT}/src)
#set(CONF_INCLUDE_DIR ${LIBGPIOD_ROOT}/include)

set(TEST_SOURCES test/test.c)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME}connection_test ${TEST_SOURCES})
#
#target_include_directories(${PROJECT_NAME}
#        PUBLIC
#        ${CMAKE_CURRENT_LIST_DIR}
#        PRIVATE
#        ${CONF_INCLUDE_DIR}
#        )
