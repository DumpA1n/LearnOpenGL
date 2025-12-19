cmake_minimum_required(VERSION 3.10)

project(OpenGLRenderer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-w)

if (APPLE)
    find_package(BZip2 REQUIRED)
    find_package(ZLIB REQUIRED)
endif()

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/utils
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/imgui
    ${CMAKE_SOURCE_DIR}/include/imgui/backends
    ${CMAKE_SOURCE_DIR}/include/freetype
    # ${CMAKE_SOURCE_DIR}/include/assimp
)

file(GLOB_RECURSE IMGUI_SOURCES
    ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/backends/*.cpp
)

file(GLOB_RECURSE STB_SOURCES
    ${CMAKE_SOURCE_DIR}/include/stb/*.cpp
)

set(SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Application.cpp
    ${CMAKE_SOURCE_DIR}/src/Core/Renderer.cpp
    ${CMAKE_SOURCE_DIR}/src/Resources/Shader.cpp
    ${CMAKE_SOURCE_DIR}/src/Resources/Geometry.cpp
    ${CMAKE_SOURCE_DIR}/src/Resources/ResourceManager.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene/Entity.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene/Transform.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene/MeshRenderer.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene/CameraComponent.cpp
    ${CMAKE_SOURCE_DIR}/src/Scene/Scene.cpp
    ${CMAKE_SOURCE_DIR}/src/MTFilter.cpp
    ${CMAKE_SOURCE_DIR}/include/glad/glad.c
    ${IMGUI_SOURCES}
    ${STB_SOURCES}
)

message(STATUS "SOURCES: ${SOURCES}")

add_executable(${PROJECT_NAME} ${SOURCES})


if (CMAKE_SYSTEM_PROCESSOR STREQUAL "arm64")
    set(ARCH "lib-arm64")
elseif(CMAKE_SYSTEM_PROCESSOR STREQUAL "x86_64")
    set(ARCH "lib-x86_64")
endif()

set(LIBS
    ${CMAKE_SOURCE_DIR}/include/GLFW/${ARCH}/libglfw3.a
    ${CMAKE_SOURCE_DIR}/include/freetype/${ARCH}/libfreetype.a
    # ${CMAKE_SOURCE_DIR}/include/assimp/${ARCH}/libassimp.a
)

if (APPLE)
    target_link_libraries(${PROJECT_NAME} ${LIBS} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES} "-framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL")
elseif (WIN32)
    target_link_libraries(${PROJECT_NAME} ${LIBS} opengl32)
endif()