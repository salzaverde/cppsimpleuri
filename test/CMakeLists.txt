cmake_minimum_required(VERSION 3.18.0)

### Fetch Google Test
include(FetchContent)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON CACHE INTERNAL "Do not update fetched content")

FetchContent_Declare(
    gtest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.12.0
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(gtest)
include(GoogleTest)

### Setup
set(target_name cppsimpleurilib_tests)
set(lib_target_name cppsimpleurilib)

add_executable(${target_name})
set_target_properties(${target_name} PROPERTIES 
    CXX_CLANG_TIDY "clang-tidy;-style=file;-checks=*"
    CXX_CPPLINT "cpplint"
    CXX_INCLUDE_WHAT_YOU_USE ON
    CXX_LINK_WHAT_YOU_USE ON
    POSITION_INDEPENDENT_CODE ON
)

target_sources(${target_name} PRIVATE
    "uri_test.cpp"
    "query_test.cpp"
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