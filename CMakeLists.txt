cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

### Project
project(cppsimpleurilib VERSION 1.0.0 LANGUAGES C CXX)

### Build generation options
option(SIMPLEURI_LIB_BUILD_TESTS "Build the unit tests." ON)
set(CMAKE_XCODE_ATTRIBUTE_ARCHS "$(ARCHS_STANDARD)")

### Library
set(lib_target_name cppsimpleurilib)
add_library(${lib_target_name} STATIC)

### Sources & Includes
target_sources(${lib_target_name} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/uri.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/query.cpp"
)

#headers
target_sources(${lib_target_name} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/salzaverde/uri.h"
)

target_include_directories(${lib_target_name} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

### Properties & Definitions
set_target_properties(${lib_target_name} PROPERTIES CXX_STANDARD 20)

### IDE structure
get_target_property(target_sources ${lib_target_name} SOURCES)
source_group(TREE "${CMAKE_SOURCE_DIR}" PREFIX "" FILES
    ${target_sources}
)

### Tests
if(SIMPLEURI_LIB_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()