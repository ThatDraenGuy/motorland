project(gpio_lib C)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

include(ExternalProject)

set(LIBGPIOD_ROOT ${CMAKE_BINARY_DIR}/external)
set(LIBGPIOD_LIB_DIR ${LIBGPIOD_ROOT}/lib)
set(LIBGPIOD_INCLUDE_DIRS ${LIBGPIOD_ROOT}/include)
set(TARGET_HOST arm-openipc-linux-musleabi)

ExternalProject_Add(
        libgpiod_external
        PREFIX ${LIBGPIOD_ROOT}
        GIT_REPOSITORY "https://git.kernel.org/pub/scm/libs/libgpiod/libgpiod.git"
        GIT_TAG "v1.6.3"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        BINARY_DIR ${LIBGPIOD_ROOT}/src/libgpiod
        SOURCE_DIR ${LIBGPIOD_ROOT}/src/libgpiod
        INSTALL_DIR ${LIBGPIOD_ROOT}/bin
        CONFIGURE_COMMAND <SOURCE_DIR>/configure
        --prefix=${LIBGPIOD_ROOT}
        --enable-tools=no
        --enable-bindings-python=no
        --host=${TARGET_HOST}
        --target=${TARGET_HOST}
        BUILD_COMMAND ${MAKE} ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi-
        BUILD_BYPRODUCTS ${LIBGPIOD_LIB_DIR}/libgpiod.a
)

ExternalProject_Add_Step(libgpiod_external
        autogen
        COMMAND ./autogen.sh --host=${TARGET_HOST} --target=${TARGET_HOST}
        DEPENDEES download
        DEPENDERS configure
        WORKING_DIRECTORY ${LIBGPIOD_ROOT}/src/libgpiod)

# dependency
add_library(libgpiod STATIC IMPORTED)
set_target_properties(libgpiod PROPERTIES IMPORTED_LOCATION ${LIBGPIOD_LIB_DIR}/libgpiod.a)
add_dependencies(libgpiod libgpiod_external)

add_executable(${PROJECT_NAME} main.c)
target_sources(${PROJECT_NAME}
        PRIVATE
        ${SOURCES}
        )

target_include_directories(${PROJECT_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        PRIVATE ${LIBGPIOD_INCLUDE_DIRS}
        )

target_link_libraries(${PROJECT_NAME} LINK_PRIVATE libgpiod common)
