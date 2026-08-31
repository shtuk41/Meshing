add_executable(PolygonizeVolume 
	main.cpp
	ioData.cpp
	ioData.h
	${CMAKE_SOURCE_DIR}/MarchingCubes/MarchingCubes.cpp
)

target_include_directories(PolygonizeVolume PRIVATE 
	${CMAKE_SOURCE_DIR}/PolygonizeVolume
	${CMAKE_SOURCE_DIR}/MarchingCubes
)

if(WIN32)
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
elseif(LINUX)
    set(CMAKE_CXX_STANDARD 20)
	set(CMAKE_CXX_STANDARD_REQUIRED ON)
	set(CMAKE_CXX_EXTENSIONS OFF)

    target_compile_features(PolygonizeVolume PRIVATE cxx_std_20)
    
	set_target_properties(PolygonizeVolume PROPERTIES
	    ARCHIVE_OUTPUT_DIRECTORY         "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_DEBUG   "${CMAKE_BINARY_DIR}/bin"
        ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY 		 "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY 		 "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY 		 "${CMAKE_BINARY_DIR}/bin"
	)
endif()










