cmake_minimum_required(VERSION 3.10)

project(uuid-new-formats)

file(GLOB LIB_SOURCES ./src/*.c)
list(REMOVE_ITEM LIB_SOURCES ./src/main.c)

file(GLOB MAIN_SOURCES ./src/*.c)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -ansi -std=c99)

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
  add_compile_options(-O2)
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

add_executable(uuid-new-formats ${MAIN_SOURCES})

add_library(uuid-new-formats-lib SHARED ${LIB_SOURCES})

target_include_directories(uuid-new-formats PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

