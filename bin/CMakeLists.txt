cmake_minimum_required(VERSION 3.10)

# Set project name
project(ACE_compiler)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add executable
add_executable(compiler main.cpp)
