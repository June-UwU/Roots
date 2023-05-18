cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME, "Roots_Interpreter")

file(GLOB_RECURSE common_inc ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE common_src ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${common_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${common_inc})

file(GLOB_RECURSE tooling_inc ${CMAKE_CURRENT_SOURCE_DIR}/tools/*.hpp)
file(GLOB_RECURSE tooling_src ${CMAKE_CURRENT_SOURCE_DIR}/tools/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tooling_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${tooling_src})


file(GLOB_RECURSE run_time_inc ${CMAKE_CURRENT_SOURCE_DIR}/runtime/*.hpp)
file(GLOB_RECURSE run_time_src ${CMAKE_CURRENT_SOURCE_DIR}/runtime/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${run_time_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${run_time_src})

file(GLOB_RECURSE memory_inc ${CMAKE_CURRENT_SOURCE_DIR}/memory/*.hpp)
file(GLOB_RECURSE memory_src ${CMAKE_CURRENT_SOURCE_DIR}/memory/*.cpp)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${memory_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${memory_src})

add_executable( ${PROJECT_NAME}
    ${common_inc}
    ${common_src}
    ${tooling_inc}
    ${tooling_src}
    ${run_time_inc}
    ${run_time_src}
    ${memory_inc}
    ${memory_src}
    )

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/generics/)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/tools/)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/memory/)