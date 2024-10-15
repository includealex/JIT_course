cmake_minimum_required(VERSION 3.16.3)
project(JITTest)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRECTORIES})

enable_testing()

file(GLOB_RECURSE JIT_TEST_SOURCES fact_test.cpp dfs_test.cpp)
set(EXEC_NAME jit_test)

add_executable(${EXEC_NAME} ${JIT_TEST_SOURCES})

if(SANITIZERS)
	target_compile_options(${EXEC_NAME} PUBLIC -fsanitize=address -g)
	set_target_properties(${EXEC_NAME} PROPERTIES LINK_FLAGS "-fsanitize=address")
endif()

target_link_libraries(${EXEC_NAME} GTest::Main my_jit)

add_custom_target(run_${EXEC_NAME}
	  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${EXEC_NAME})

add_dependencies(run_${EXEC_NAME} ${EXEC_NAME})
