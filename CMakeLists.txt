cmake_minimum_required(VERSION 3.19.0 FATAL_ERROR)
set_property(GLOBAL PROPERTY USE_FOLDERS YES)

### Project
project(cppsimpleuri VERSION 1.0.0 LANGUAGES C CXX)

### Build generation options
option(SIMPLEURI_LIB_BUILD_TESTS "Build the unit tests." ON)
option(SIMPLEURI_LIB_BUILD_EXAMPLES "Build examples." OFF)
option(SIMPLEURI_LIB_CODE_COVERAGE "Generate a code coverage report using LCOV." OFF)

set(CMAKE_XCODE_ATTRIBUTE_ARCHS "$(ARCHS_STANDARD)")

### Library
set(lib_target_name cppsimpleuri)
add_library(${lib_target_name} STATIC)

### Sources & Includes
target_sources(${lib_target_name} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/src/uri.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/query.cpp"
)

#headers
target_sources(${lib_target_name} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/salzaverde/uri.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/salzaverde/query.h"
)

target_include_directories(${lib_target_name} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

### Properties & Definitions
set_target_properties(${lib_target_name} PROPERTIES
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
)

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

### Examples
if(SIMPLEURI_LIB_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()