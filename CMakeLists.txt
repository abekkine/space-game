cmake_minimum_required(VERSION 3.15)

project(spaceGame)

add_compile_options(-g -Wall -Wfatal-errors)

include_directories($ENV{EXT_LIB_DIR}/nlohmann-json)
include_directories($ENV{EXT_LIB_DIR}/glfw-3.3.2/include)
include_directories($ENV{EXT_LIB_DIR}/box2d/include)
include_directories($ENV{EXT_LIB_DIR}/ftgl/include)
include_directories($ENV{EXT_LIB_DIR}/freetype/include)

link_directories($ENV{EXT_LIB_DIR}/ftgl/lib)
link_directories($ENV{EXT_LIB_DIR}/box2d/lib)
link_directories($ENV{EXT_LIB_DIR}/glfw-3.3.2/lib-mingw-w64)

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/interfaces)
include_directories(${CMAKE_SOURCE_DIR}/systems/inc)

file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/systems/src/*.cpp
)

add_executable(spaceGame ${SOURCES})
target_link_libraries(spaceGame
    glfw3 opengl32
    box2d
    ftgl.dll
)
