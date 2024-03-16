cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME, "roots_lang")

file(GLOB_RECURSE common_inc ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE common_src ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${common_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${common_src})

file(GLOB_RECURSE include_inc ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${include_inc})


add_library( ${PROJECT_NAME} STATIC
    ${common_src}
    ${common_inc}
    ${include_inc}
 "lexer.cxx" "repl.cxx")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
