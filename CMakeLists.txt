cmake_minimum_required(VERSION 3.10)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -ansi -std=c99)
add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")

project(uuid-new-formats)

file (GLOB SOURCES ./src/*.c)

include_directories (${CMAKE_CURRENT_LIST_DIR}/include)

add_executable (uuid-new-formats ${SOURCES})
target_include_directories (uuid-new-formats PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

