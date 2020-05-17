cmake_minimum_required( VERSION 3.8)

add_executable(Lab_10 main.cpp)

if(MSVC)
add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()