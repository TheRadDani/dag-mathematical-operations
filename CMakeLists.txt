# Minimum CMake version required
cmake_minimum_required(VERSION 3.10)

# Project name
project(DAG_Evaluator)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable for your program
add_executable(dag_evaluator dag.cpp)
