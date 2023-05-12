project(gpio_lib C)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")
file(GLOB_RECURSE HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/*.h")

include_directories(include)


add_executable(${PROJECT_NAME} main.c)
#add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
        PRIVATE
        ${SOURCES}
        PUBLIC
        ${HEADERS}
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        PRIVATE
        ./libs/libgpiod-1.6.3/include
        )
