project(motorsd C)

set(SOURCES src/daemon.c)
set(TEST_SOURCES test/test.c)

add_executable(${PROJECT_NAME} ${SOURCES})
add_executable(${PROJECT_NAME}tests ${TEST_SOURCES})
