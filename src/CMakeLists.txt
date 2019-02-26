set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")

##### TechTree Lib
file(GLOB techtree_sources ./TechTree/*.cpp ./TechTree/*.h)
add_library(TechTreeLib ${techtree_sources})

target_include_directories(TechTreeLib
    PUBLIC ${PROJECT_SOURCE_DIR}/include 
    PUBLIC ${s2lib_INCLUDES}

    PUBLIC ./TechTree
)
set_property(TARGET TechTreeLib PROPERTY CXX_STANDARD 17)
target_link_libraries(TechTreeLib 
	PUBLIC ${s2lib_LIBRARIES}
	PUBLIC nlohmann_json::nlohmann_json
)


##### TechTree Executable
add_executable(TechTreeGenerator apps/TechTreeMain.cpp)
target_link_libraries(TechTreeGenerator 
    PUBLIC TechTreeLib
    PRIVATE nlohmann_json::nlohmann_json
)
target_compile_features(TechTreeGenerator PRIVATE cxx_std_17)


##### BoBuilder Executable
file(GLOB bobuilder_sources ./BuildOrder/*.cpp ./BuildOrder/*.h)
add_executable(BoBuilder apps/BoBuilder.cpp ${bobuilder_sources})
target_include_directories(BoBuilder
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	PUBLIC ${s2lib_INCLUDES}
	PUBLIC ./BuildOrder
)
target_link_libraries(BoBuilder
	PUBLIC TechTreeLib
)
target_compile_features(BoBuilder PRIVATE cxx_std_17)