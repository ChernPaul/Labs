

cmake_minimum_required (VERSION 3.8)

project(tmp CXX)

if ("${CMAKE_CXX_COMPILER_ID})" STREQUAL "GNU")

add_compile_options(-Wall -Wextra -Wpedantic)

elseif("${CMAKE_CXX_COMPILER_ID})" STREQUAL "MSVC")

add_compile_options(/W4 /D_CRT_SECURE_NO_WARNINGS)

endif()

add_subdirectory ("Sort_6")
add_subdirectory ("Containers_8")
add_subdirectory ("Graphs_10")
