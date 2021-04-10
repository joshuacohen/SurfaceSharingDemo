cmake_minimum_required(VERSION 3.1...3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SharedSurface)

add_executable(SharedSurface WIN32 "src/Main.cpp")