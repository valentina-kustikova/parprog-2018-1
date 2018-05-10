cmake_minimum_required(VERSION 3.8)
project(Dijkstra_s_algorithm_OpenMP_TBB)

set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp typer.cpp viewer.cpp before_code.cpp)
add_executable(Dijkstra_s_algorithm_OpenMP_TBB ${SOURCE_FILES})