cmake_minimum_required(VERSION 3.1...3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(SharedSurface)

add_executable(SharedSurface WIN32 "src/Main.cpp" "src/D3D11App.cpp")
include_directories(SharedSurface PRIVATE "src/inc")
target_link_libraries(SharedSurface PRIVATE "dxgi.lib" "d3d11.lib")