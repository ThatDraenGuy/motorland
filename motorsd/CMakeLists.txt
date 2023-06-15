project(motorsd C)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")
include_directories(include)

#set()

set(TEST_SOURCES test/test.c)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME}connection_test ${TEST_SOURCES})
