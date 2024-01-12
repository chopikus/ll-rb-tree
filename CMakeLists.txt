cmake_minimum_required(VERSION 3.10)
project(ll-rb-tree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall -O2 -fsanitize=address")

add_executable(USAGE_EXAMPLE usage_example.cpp)
set_target_properties(USAGE_EXAMPLE PROPERTIES OUTPUT_NAME "usage_example")

target_link_libraries(USAGE_EXAMPLE)

# setup testing
link_directories(/usr/local/lib)

enable_testing()
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIR})

add_custom_target(tests COMMAND GTEST_COLOR=yes ${CMAKE_CTEST_COMMAND} --verbose)

file(GLOB TEST_FILES "tests/*.cpp")
add_executable("ll_rb_tree_tests" EXCLUDE_FROM_ALL ${TEST_FILES})
target_link_libraries("ll_rb_tree_tests" gtest pthread)
add_dependencies(tests "ll_rb_tree_tests")
add_test("ll_rb_tree_tests" "./ll_rb_tree_tests")
