project(ofApp)

cmake_minimum_required(VERSION 2.8)

set(OF_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../of/" CACHE PATH "The root directory of ofnode/of project.")
include(${OF_ROOT}/openFrameworks.cmake)

ofxaddon("ofxGui")

set(SOURCES
        src/main.cpp
        src/ofApp.cpp
        src/Pendulum.h src/segment.h src/segment.cpp src/Pendulum.cpp)

add_executable(
    ${PROJECT_NAME}
    MACOSX_BUNDLE
    ${SOURCES}
    ${OFXADDONS_SOURCES}
)

set(CMAKE_STATIC_LINKER_FLAGS "-ljack")

set(CMAKE_EXE_LINKER_FLAGS "-ljack")


target_link_libraries(
        ${PROJECT_NAME}
        ${OPENFRAMEWORKS_LIBRARIES}
)

ofSetTargetProperties()
