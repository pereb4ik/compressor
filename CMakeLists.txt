cmake_minimum_required(VERSION 3.15)
project(SemestrWork C)

set(CMAKE_C_STANDARD 11)

find_package(GLUT)

add_executable(SemestrWork compressor.c lol.h)