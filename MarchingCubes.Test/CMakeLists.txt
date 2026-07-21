find_package(GTest CONFIG REQUIRED)

set (SHADERS
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/shaders/cube.frag"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/shaders/cube.vert"
)

set (HEADERS
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/camera.h"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/cube.h"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/render_object.h"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/shaders.h"
)

set (MSOURCES
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/camera.cpp"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/cube.cpp"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/render_object.cpp"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/shaders.cpp"
	"${CMAKE_SOURCE_DIR}/MarchingCubes.Test/test.cpp"
)

source_group("Source Files" FILES ${MSOURCES})
source_group("Source Files\\Shaders" FILES ${SHADERS})
source_group("Header Files" FILES ${HEADERS})


add_executable(MarchingCubes.Test ${MSOURCES} ${SHADERS})

target_sources(MarchingCubes.Test PRIVATE
				${MSOURCES}
				${HEADERS}
)

target_include_directories(MarchingCubes.Test PRIVATE 
	${CMAKE_SOURCE_DIR}/MarchingCubes.Test
	${CMAKE_SOURCE_DIR}/MarchingCubes)

target_link_libraries(MarchingCubes.Test
    PRIVATE 
        GTest::gtest_main   # Automatically provides a standard main() entry point
        GTest::gtest        # The core GoogleTest library
        MarchingCubesLib
		glfw
		glm::glm
		GLEW::GLEW
		opengl32
)

target_compile_features(MarchingCubes.Test PRIVATE cxx_std_20)

add_custom_command(TARGET MarchingCubes.Test POST_BUILD

    # Copy shaders directory to both locations
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/MarchingCubes.Test/shaders" "${CMAKE_BINARY_DIR}/MarchingCubes.Test/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/MarchingCubes.Test/shaders" "${CMAKE_BINARY_DIR}/x64/$<CONFIG>/shaders"
)


add_test(NAME MarchingCubes.Test COMMAND MarchingCubeTest)