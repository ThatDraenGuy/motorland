project(camhi_uart_lib C)

add_executable(test_${PROJECT_NAME} main.c src/camhi_uart_lib.c)
add_library(${PROJECT_NAME} src/camhi_uart_lib.c)

target_link_libraries(${PROJECT_NAME} PUBLIC common)
target_link_libraries(test_${PROJECT_NAME} PUBLIC common)
