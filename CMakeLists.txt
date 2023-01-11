# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("LearnOpenGL")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

file(GLOB GLAD ${CMAKE_CURRENT_SOURCE_DIR}/src/glad/glad.c)
file(GLOB IMGUI ${CMAKE_CURRENT_SOURCE_DIR}/ImGui/*.cpp)
file(GLOB SRC_LIST_CPP ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB DLL ${CMAKE_SOURCE_DIR}/lib/*.dll)

file(COPY ${DLL} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/shader/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shader)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets/ DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/lib)

include_directories (${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories (${CMAKE_CURRENT_SOURCE_DIR}/ImGui)
link_directories (${LIB_DIR})
link_libraries (glfw3 opengl32 assimp-vc142-mtd)

add_library(glad_lib ${GLAD})
add_library(imgui_lib ${IMGUI})
add_library(src_lib ${SRC_LIST_CPP})

# 包含子项目。
add_subdirectory ("LearnOpenGL")
