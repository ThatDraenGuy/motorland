include(ExternalProject)

set(LIBCYAML_ROOT ${CMAKE_BINARY_DIR}/external)
set(LIBCYAML_LIB_DIR ${LIBCYAML_ROOT}/lib)
set(LIBCYAML_INCLUDE_DIRS ${LIBCYAML_ROOT}/include)
set(TARGET_HOST arm-openipc-linux-musleabi)

ExternalProject_Add(
        libcyaml_external
        PREFIX ${LIBCYAML_ROOT}
        GIT_REPOSITORY "https://github.com/tlsa/libcyaml.git"
        GIT_TAG "v1.4.1"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        BINARY_DIR ${LIBCYAML_ROOT}/src/libcyaml
        SOURCE_DIR ${LIBCYAML_ROOT}/src/libcyaml
        INSTALL_DIR ${LIBCYAML_ROOT}/bin
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${MAKE} ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi-
        BUILD_BYPRODUCTS ${LIBCYAML_LIB_DIR}/libcyaml.a
        INSTALL_COMMAND make PREFIX=${LIBCYAML_ROOT} install
)

add_library(libcyaml STATIC IMPORTED)
set_target_properties(libcyaml PROPERTIES IMPORTED_LOCATION ${LIBCYAML_LIB_DIR}/libcyaml.a)
add_dependencies(libcyaml libcyaml_external)

