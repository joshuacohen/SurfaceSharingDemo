cmake_minimum_required(VERSION 3.1...3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SharedSurface11 WIN32 "Main.cpp" "D3D11App.cpp")
include_directories(SharedSurface11 PRIVATE "inc")
target_link_libraries(SharedSurface11 PRIVATE "dxgi.lib" "d3d11.lib")