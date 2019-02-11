cmake_minimum_required(VERSION 3.13)

set(PROJECT_NAME YoYoBot)

project(${PROJECT_NAME}
    VERSION 0.0.1
    LANGUAGES CXX
    )

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib )

# Setup Platform
if(NOT DEFINED CMAKE_GENERATOR_PLATFORM) 
    set(CMAKE_GENERATOR_PLATFORM x64)
endif(NOT DEFINED CMAKE_GENERATOR_PLATFORM)

#Configure s2client-api include and library directories
set(s2lib_LIBRARIES_DIRS ./s2client-api/build/bin)
link_directories(${s2lib_LIBRARIES_DIRS})
set(s2lib_INCLUDES
    ${PROJECT_SOURCE_DIR}/s2client-api/include
    ${PROJECT_SOURCE_DIR}/s2client-api/build/generated
    ${PROJECT_SOURCE_DIR}/s2client-api/contrib/SDL-mirror/include
    ${PROJECT_SOURCE_DIR}/s2client-api/contrib/civetweb/include
    ${PROJECT_SOURCE_DIR}/s2client-api/contrib/protobuf/src
)
include_directories(${s2lib_INCLUDES})

set(s2lib_LIBRARIES sc2apid.lib sc2libd.lib sc2utilsd.lib sc2rendererd.lib sc2protocold.lib libprotobufd.lib civetweb.lib)

add_subdirectory(src)
add_subdirectory(YoBot)
add_subdirectory(test)