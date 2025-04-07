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

file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/main.cpp
    ${CMAKE_SOURCE_DIR}/src/shader.cpp
    ${CMAKE_SOURCE_DIR}/include/glad/*.c
    ${CMAKE_SOURCE_DIR}/include/stb/*.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/*.cpp
    ${CMAKE_SOURCE_DIR}/include/imgui/backends/*.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})


if (APPLE)
    set(ARCH "lib-MACOS")
elseif (WIN32)
    set(ARCH "lib-WIN32")
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