

add_library(MarchingCubes STATIC
	MarchingCubes.cpp
	MarchingCubes.h
)

target_include_directories(MarchingCubes PRIVATE 
	${CMAKE_SOURCE_DIR}/MarchingCubes
)

set_target_properties(MarchingCubes PROPERTIES
    # Executables
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>"
    
    # Static Libraries (.lib/.a)
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>"
    
    # Shared Libraries/DLLs
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>"
    
    # Debug Symbols
    PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/x64/$<CONFIG>"
    
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)






