cmake_minimum_required(VERSION 3.15)

# Project Name and Language
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add personal header from (./myheader)
include_directories(myheader)

# Add executable
add_executable(main
    src/main.cpp
    src/Books.cpp
    src/DBTools.cpp
    src/Helper.cpp
    src/Library.cpp
    src/Students.cpp
)