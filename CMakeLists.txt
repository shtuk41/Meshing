cmake_minimum_required(VERSION 3.16)

#VCPGK

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_USE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)
set(VCPKG_FORCE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)

project(Meshing VERSION 0.1 LANGUAGES CXX CUDA)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_subdirectory(MarchingCubes)





