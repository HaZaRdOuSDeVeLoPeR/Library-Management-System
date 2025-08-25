cmake_minimum_required(VERSION 3.15)

# Project Name and Language
project(MyProject VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add personal header from (./myheader)
include_directories(myheader)

# Add external header from (./extheader)
# include_directories(extheader/extheader.h)

# Link with static libraries from (stdlibrary or from ./slib)
# target_link_libraries(MyProject PRIVATE pthread slib/slib.a)

# Add executable
add_executable(main
    src/main.cpp
    src/Books.cpp
    src/DBTools.cpp
    src/Helper.cpp
    src/Library.cpp
    src/Students.cpp
)

# Enable testing
# enable_testing()
# add_executable(MyTest tests/test.cpp)
# add_test(NAME MyTest COMMAND MyTest)

# Install rules
# install(TARGETS MyProject DESTINATION bin)