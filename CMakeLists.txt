cmake_minimum_required(VERSION 3.28)
project(riscvemulator)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/dependencies)

file(GLOB_RECURSE SOURCES "./*.c")

add_executable(riscvemulator ${SOURCES})
target_link_libraries(riscvemulator m)