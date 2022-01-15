cmake_minimum_required(VERSION 3.10)

project(uuid-new-formats)

include_directories(${CMAKE_CURRENT_LIST_DIR}/include)

# Select and set random files
IF(UNIX)
  file(GLOB RANDOM_SOURCES ./src/linux-random/*.c)
ELSE()
  file(GLOB RANDOM_SOURCES ./src/pcg32-random/*.c)
ENDIF()

# Set library file
file(GLOB LIB_SOURCES ./src/uuid67.c)

# Set executable application files
file(GLOB MAIN_SOURCES ./src/*.c)

add_compile_options(-Wall -Wextra -Werror -Wpedantic -ansi -std=c99)

IF(CMAKE_BUILD_TYPE MATCHES RELEASE)
  add_compile_options(-O2)
ENDIF(CMAKE_BUILD_TYPE MATCHES RELEASE)

IF(CMAKE_BUILD_TYPE MATCHES DEBUG)
  add_compile_options("$<$<CONFIG:DEBUG>:-DDEBUG>")
ENDIF(CMAKE_BUILD_TYPE MATCHES DEBUG)

# Add builded library and executable
add_executable(uuid-new-formats ${MAIN_SOURCES} ${RANDOM_SOURCES})
add_library(uuid-new-formats-lib SHARED ${LIB_SOURCES} ${RANDOM_SOURCES})

target_include_directories(uuid-new-formats PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_include_directories(uuid-new-formats-lib PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

