project(motorsd C)

set(SOURCES src/daemon.c)

add_executable(${PROJECT_NAME} ${SOURCES})
