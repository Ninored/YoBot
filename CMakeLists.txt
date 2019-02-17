cmake_minimum_required(VERSION 3.12)

# Project name
set(PROJECT_NAME YoBot)
project(${PROJECT_NAME}
    VERSION 0.0.1
    LANGUAGES CXX
    )

# Configure output directories
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/build/lib )

#Configure s2client-api include and library directories
set(s2lib_LIBRARIES_DIRS ${PROJECT_SOURCE_DIR}/thirdparty/s2client-api/build/bin)
set(s2lib_INCLUDES
    ${PROJECT_SOURCE_DIR}/thirdparty/s2client-api/include
    ${PROJECT_SOURCE_DIR}/thirdparty/s2client-api/build/generated
    ${PROJECT_SOURCE_DIR}/thirdparty/s2client-api/contrib/SDL-mirror/include
    ${PROJECT_SOURCE_DIR}/thirdparty/s2client-api/contrib/civetweb/include
    ${PROJECT_SOURCE_DIR}/thirdparty/s2client-api/contrib/protobuf/src
)
set(s2lib_LIBRARIES sc2apid.lib sc2libd.lib sc2utilsd.lib sc2rendererd.lib sc2protocold.lib libprotobufd.lib civetweb.lib)

link_directories(${s2lib_LIBRARIES_DIRS})
include_directories(${s2lib_INCLUDES})

# Sub directories
add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(YoBot)
add_subdirectory(test)