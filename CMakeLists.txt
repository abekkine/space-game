cmake_minimum_required(VERSION 3.15)

project(spaceGame)

add_compile_options(-g -Wfatal-errors)

include_directories($ENV{EXT_LIB_DIR}/nlohmann-json)

if (UNIX)
    include_directories(/usr/include/freetype2)
else()
    include_directories($ENV{EXT_LIB_DIR}/glfw-3.3.2/include)
    include_directories($ENV{EXT_LIB_DIR}/box2d/include)
    include_directories($ENV{EXT_LIB_DIR}/ftgl/include)
    include_directories($ENV{EXT_LIB_DIR}/freetype/include)

    link_directories($ENV{EXT_LIB_DIR}/ftgl/lib)
    link_directories($ENV{EXT_LIB_DIR}/box2d/lib)
    link_directories($ENV{EXT_LIB_DIR}/glfw-3.3.2/lib-mingw-w64)
endif()


include_directories(${CMAKE_SOURCE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/interfaces)
include_directories(${CMAKE_SOURCE_DIR}/systems/inc)

file(GLOB SOURCES 
    ${CMAKE_SOURCE_DIR}/*.cpp
    ${CMAKE_SOURCE_DIR}/systems/src/*.cpp
)

add_executable(spaceGame ${SOURCES})
if (UNIX)
    target_link_libraries(spaceGame
        pthread
        GL
        glfw
        Box2D
        ftgl
    )
else()
    target_link_libraries(spaceGame
        glfw3 opengl32
        box2d
        ftgl.dll
    )
endif()
