cmake_minimum_required(VERSION 3.30.5)
project(JITLib VERSION 1.0.0 DESCRIPTION "JIT library")

file(GLOB_RECURSE ALL_CXX_SOURCE_FILES ${PROJECT_SOURCE_DIR}/*.[ch]pp)

find_program(CLANG_FORMAT "clang-format" REQUIRED)

add_custom_target (
    format
    COMMAND clang-format
    -i
    -style=file
    ${ALL_CXX_SOURCE_FILES}
)

add_library(my_jit STATIC ${ALL_CXX_SOURCE_FILES})

target_include_directories(my_jit PUBLIC includes/)

add_subdirectory(tests)

set_target_properties (my_jit PROPERTIES
    CXX_STANDARD          17
    CXX_STANDARD_REQUIRED ON
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")