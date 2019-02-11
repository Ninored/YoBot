file(GLOB TechTreeTests_SOURCES TechTreeTests/*.cpp TechTreeTests/*.hpp)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
include_directories(.)


add_executable(TechTreeTests ${TechTreeTests_SOURCES})
target_link_libraries(TechTreeTests TechTreeLib)