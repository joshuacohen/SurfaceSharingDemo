cmake_minimum_required(VERSION 3.1...3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(HelloTriangle WIN32 D3D12HelloTriangle.cpp DXSample.cpp Main.cpp stdafx.cpp Win32Application.cpp)
add_compile_definitions(UNICODE)
include_directories(HelloTriangle PRIVATE inc)
target_link_libraries(HelloTriangle PRIVATE "dxgi.lib" "D3D12.lib" "D3DCompiler.lib")

add_custom_target(CopyShaders ALL DEPENDS always_rebuild)
add_custom_command(TARGET CopyShaders HelloTriangle POST_BUILD COMMAND robocopy ARGS /s "${CMAKE_CURRENT_SOURCE_DIR}/shaders" "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}")
