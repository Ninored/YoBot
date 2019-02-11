file(GLOB yobot_sources *.cpp *.h)

set(CMAKE_CXX_FLAGS_RELEASE "/MT")
set(CMAKE_CXX_FLAGS_DEBUG "/MTd")
add_executable(YoBot ${yobot_sources})

target_include_directories(YoBot PUBLIC ${s2lib_INCLUDES})
target_link_libraries(YoBot PUBLIC ${s2lib_LIBRARIES})
target_compile_features(YoBot PRIVATE cxx_std_14)
set_property(TARGET YoBot PROPERTY CXX_STANDARD 14)
set_property(TARGET YoBot PROPERTY CXX_STANDARD_REQUIRED ON)
