cmake_minimum_required(VERSION 3.5)

project(INI_file_parser)

set(CMAKE_CXX_STANDARD 17)

add_executable(INI_file_parser main.cpp)

target_compile_options(INI_file_parser PRIVATE -Wall -Wextra -pedantic)


