##### TechTree Lib
file(GLOB techtree_sources ./TechTree/*.cpp ./TechTree/*.h)
add_library(TechTreeLib ${techtree_sources})
target_link_libraries(TechTreeLib PUBLIC ${s2lib_LIBRARIES} PUBLIC nlohmann_json::nlohmann_json)
target_include_directories(TechTreeLib
    PUBLIC ${PROJECT_SOURCE_DIR}/include 
    PUBLIC ${PROJECT_SOURCE_DIR}/src/TechTree
)
set_property(TARGET TechTreeLib PROPERTY CXX_STANDARD 17)

##### TechTree Executable
add_executable(TechTreeGenerator apps/TechTreeMain.cpp)
target_link_libraries(TechTreeGenerator 
    PRIVATE TechTreeLib
    PRIVATE nlohmann_json::nlohmann_json
)
target_compile_features(TechTreeGenerator PRIVATE cxx_std_17)


##### BoBuilder Executable
file(GLOB_RECURSE simulator_sources 
	${PROJECT_SOURCE_DIR}/src/Simulator/*.cpp 
	${PROJECT_SOURCE_DIR}/src/Simulator/*.h
)
add_executable(Simulator apps/Simulation.cpp ${simulator_sources})

target_include_directories(Simulator
	PUBLIC ${PROJECT_SOURCE_DIR}/include
	PUBLIC ${PROJECT_SOURCE_DIR}/src/Simulator
)
target_link_libraries(Simulator PUBLIC TechTreeLib)
target_compile_features(Simulator PRIVATE cxx_std_17)