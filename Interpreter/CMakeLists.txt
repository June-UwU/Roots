cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME, "Roots_Interpreter")

file(GLOB_RECURSE common_inc ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE common_src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${common_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${common_inc})

add_executable( ${PROJECT_NAME}
    ${common_inc}
    ${common_src}
    )


