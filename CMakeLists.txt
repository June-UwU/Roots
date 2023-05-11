cmake_minimum_required(VERSION 3.20)


if (WIN32)
    add_compile_definitions(ROOTS_WIN32)
endif (WIN32)

if (UNIX)
    add_compile_definitions(ROOTS_UNIX)
endif (UNIX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PROJECT_NAME "Roots")

project(${PROJECT_NAME} LANGUAGES CXX)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
message("Cmake output dir : ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
message("Cmake library output dir : ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
message("Cmake runtime dir : ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")



set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
message("Cmake C++ standard : ${CMAKE_CXX_STANDARD}")

add_subdirectory("Interpreter")
add_custom_command(OUTPUT generating ctags database...
    COMMAND ctags -R)
