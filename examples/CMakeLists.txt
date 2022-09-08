cmake_minimum_required(VERSION 3.18.0)

set(target_name cppsimpleuri-examples)
set(lib_target_name cppsimpleuri)

add_executable(${target_name} "${CMAKE_CURRENT_SOURCE_DIR}/examples.cpp")
target_link_libraries(${target_name} PRIVATE ${lib_target_name})
set_target_properties(${target_name} PROPERTIES CXX_STANDARD 20)
