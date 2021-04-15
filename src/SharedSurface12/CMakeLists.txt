cmake_minimum_required(VERSION 3.1...3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(SharedSurface12 WIN32 "Main.cpp" "D3D12App.cpp")
include_directories(SharedSurface12 PRIVATE "inc")
target_link_libraries(SharedSurface12 PRIVATE "dxgi.lib" "D3D12.lib" "D3DCompiler.lib")

file(COPY "shaders" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})