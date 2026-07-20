

add_library(PolygonizeVolume STATIC
	main.cpp
	ioData.cpp
	ioData.h
)

target_include_directories(PolygonizeVolume PRIVATE 
	${CMAKE_SOURCE_DIR}/PolygonizeVolume
	${CMAKE_SOURCE_DIR}/MarchingCubes
)

set_target_properties(PolygonizeVolume PROPERTIES
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






