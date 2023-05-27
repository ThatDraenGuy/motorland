project(motorsd C)

# With this one i tried to access protocol_manager.h
#file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

include_directories(include)

set(SOURCES src/daemon.c src/command_parser.c src/motor_invoker.c include/motor_invoker.h)
set(TEST_SOURCES test/test.c)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME}connection_test ${TEST_SOURCES})
