file(GLOB TechTreeTests_SOURCES TechTreeTests/*.cpp TechTreeTests/*.h)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
include_directories(.)

# Configure output directories
add_executable("test-TechTree" ${TechTreeTests_SOURCES})
target_link_libraries("test-TechTree" 
	PRIVATE TechTreeLib )