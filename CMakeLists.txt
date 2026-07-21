cmake_minimum_required(VERSION 3.16)


option(BUILD_TESTING "Build the test suite" ON)

if(BUILD_TESTING)
    set(VCPKG_MANIFEST_FEATURES "tests" CACHE STRING "" FORCE)
else()
    set(VCPKG_MANIFEST_FEATURES "" CACHE STRING "" FORCE)
endif()

#VCPGK

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
set(VCPKG_USE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)
set(VCPKG_FORCE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)

project(Meshing VERSION 0.1 LANGUAGES CXX CUDA)

find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)

add_subdirectory(MarchingCubes)
add_subdirectory(PolygonizeVolume)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(MarchingCubes.Test)
endif()




