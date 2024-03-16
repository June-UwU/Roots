cmake_minimum_required(VERSION 3.20)

message("building reference ...")

set(PROJECT_NAME "reference_repl")

file(GLOB_RECURSE g_inc ${CMAKE_CURRENT_SOURCE_DIR}/*.hpp)
file(GLOB_RECURSE g_src ${CMAKE_CURRENT_SOURCE_DIR}/*.cxx)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${g_inc})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${g_src})


add_executable(${PROJECT_NAME}
	${g_inc}
	${g_src}
	)

message("Dependencies : roots_lang")
message(${CMAKE_CURRENT_SOURCE_DIR}/../roots/include/)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../roots/include/)
target_link_libraries(${PROJECT_NAME} PRIVATE roots_lang)