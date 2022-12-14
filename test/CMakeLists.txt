cmake_minimum_required(VERSION 3.18.0)

### Fetch Google Test
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Do not update fetched content")

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(googletest)
include(GoogleTest)

### Setup
set(target_name cppsimpleuri_tests)
set(lib_target_name cppsimpleuri)

add_executable(${target_name})
target_sources(${target_name} PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/uri_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/query_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/encoder_test.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/config/uri_components.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/cartesian_product.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/detail/uri_generator.h"
)

target_include_directories(${lib_target_name} PUBLIC 
    "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_libraries(${target_name} PRIVATE
    gtest_main
    ${lib_target_name}
)

set_target_properties(${target_name} PROPERTIES CXX_STANDARD 20)

### IDE structure
get_target_property(target_sources ${target_name} SOURCES)
source_group(TREE "${CMAKE_SOURCE_DIR}" PREFIX "" FILES
    ${target_sources}
)

set_target_properties(gtest PROPERTIES FOLDER Extern)
set_target_properties(gtest_main PROPERTIES FOLDER Extern)
set_target_properties(gmock PROPERTIES FOLDER Extern)
set_target_properties(gmock_main PROPERTIES FOLDER Extern)

gtest_discover_tests(${target_name})

###Coverage
if(SIMPLEURI_LIB_CODE_COVERAGE)
    find_program(LCOV_TOOL NAMES lcov REQUIRED)

    set(coverage_include
        ${PROJECT_SOURCE_DIR}/include/salzaverde/*.h
        ${PROJECT_SOURCE_DIR}/include/salzaverde/detail/*.h
        ${PROJECT_SOURCE_DIR}/src/*.cpp
        ${PROJECT_SOURCE_DIR}/src/detail/*.cpp
    )

    set(coverage_directory "${PROJECT_BINARY_DIR}/code_coverage")
    set(coverage_all_path ${coverage_directory}/coverage_all.info)
    set(coverage_path ${coverage_directory}/coverage.txt)

    add_custom_target(code_coverage
        COMMENT "Generating code coverage report..."

        COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_FLAGS="-fprofile-arcs;-ftest-coverage;-fno-exceptions" -S${PROJECT_SOURCE_DIR} -B${coverage_directory}
        COMMAND ${CMAKE_COMMAND} --build ${coverage_directory}
        COMMAND cd ${coverage_directory} && ctest --output-on-failure --parallel 10

        COMMAND ${LCOV_TOOL} --directory ${coverage_directory} --capture --output-file ${coverage_all_path} --rc lcov_branch_coverage=1
        COMMAND ${LCOV_TOOL} -e ${coverage_all_path} ${coverage_include} --output-file ${coverage_path} --rc lcov_branch_coverage=1
        COMMAND genhtml --title "cppsimpleuri" --legend --demangle-cpp --output-directory "${coverage_directory}/html" --show-details --branch-coverage ${coverage_path}
    )
endif()