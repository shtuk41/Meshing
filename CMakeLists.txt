cmake_minimum_required(VERSION 3.25)

if(LINUX) 
	set(CMAKE_C_COMPILER "/usr/bin/gcc" CACHE PATH "" FORCE)
	set(CMAKE_CXX_COMPILER "/usr/bin/g++" CACHE PATH "" FORCE)
endif()

option(BUILD_TESTING "Build the test suite" ON)

if(BUILD_TESTING)
    set(VCPKG_MANIFEST_FEATURES "tests" CACHE STRING "" FORCE)
else()
    set(VCPKG_MANIFEST_FEATURES "" CACHE STRING "" FORCE)
endif()

#VCPGK

if(WIN32)
	set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
elseif(LINUX)
	set(CMAKE_TOOLCHAIN_FILE "~/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

set(VCPKG_USE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)
set(VCPKG_FORCE_SYSTEM_BINARIES ON CACHE BOOL "" FORCE)

project(Meshing VERSION 0.1 LANGUAGES CXX)

find_package(GTest REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_subdirectory(MarchingCubes)
add_subdirectory(PolygonizeVolume)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(MarchingCubes.Test)
endif()




