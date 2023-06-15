include(ExternalProject)

set(LIBYAML_ROOT ${CMAKE_BINARY_DIR}/external)
set(LIBYAML_LIB_DIR ${LIBYAML_ROOT}/lib)
set(LIBYAML_INCLUDE_DIRS ${LIBYAML_ROOT}/include)
set(TARGET_HOST arm-openipc-linux-musleabi)

ExternalProject_Add(
        libyaml_external
        PREFIX ${LIBCYAML_ROOT}
        GIT_REPOSITORY "https://github.com/yaml/libyaml.git"
        GIT_TAG "0.2.5"
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
        BINARY_DIR ${LIBYAML_ROOT}/src/libyaml
        SOURCE_DIR ${LIBYAML_ROOT}/src/libyaml
        INSTALL_DIR ${LIBYAML_ROOT}/bin
        CONFIGURE_COMMAND <SOURCE_DIR>/configure
        --prefix=${LIBYAML_ROOT}
        --host=${TARGET_HOST}
        --target=${TARGET_HOST}
        BUILD_COMMAND ${MAKE} ARCH=arm CROSS_COMPILE=arm-openipc-linux-musleabi-
        BUILD_BYPRODUCTS ${LIBYAML_LIB_DIR}/libyaml.a
)

ExternalProject_Add_Step(libyaml_external
        bootstrap
        COMMAND ./bootstrap --host=${TARGET_HOST} --target=${TARGET_HOST}
        DEPENDEES download
        DEPENDERS configure
        WORKING_DIRECTORY ${LIBYAML_ROOT}/src/libyaml)

add_library(libyaml STATIC IMPORTED)
set_target_properties(libyaml PROPERTIES IMPORTED_LOCATION ${LIBYAML_LIB_DIR}/libyaml.a)
add_dependencies(libyaml libyaml_external)

