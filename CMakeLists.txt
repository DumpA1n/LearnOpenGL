cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(test)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 17)

add_compile_options(-w)

# 查找并链接必要的库
if (APPLE)
    find_package(BZip2 REQUIRED)
    find_package(ZLIB REQUIRED)
endif()

# 查找头文件
include_directories("${CMAKE_SOURCE_DIR}")
include_directories("${CMAKE_SOURCE_DIR}/demo")
include_directories("${CMAKE_SOURCE_DIR}/utils")
include_directories("${CMAKE_SOURCE_DIR}/include")
include_directories("${CMAKE_SOURCE_DIR}/include/glad")
include_directories("${CMAKE_SOURCE_DIR}/include/stb")
include_directories("${CMAKE_SOURCE_DIR}/include/imgui")
include_directories("${CMAKE_SOURCE_DIR}/include/imgui/backends")
include_directories("${CMAKE_SOURCE_DIR}/include/GLFW")
include_directories("${CMAKE_SOURCE_DIR}/include/freetype")
# include_directories("${CMAKE_SOURCE_DIR}/include/assimp")

# 添加源文件
file(GLOB_RECURSE SOURCES
    "${CMAKE_SOURCE_DIR}/main.cpp"
    "${CMAKE_SOURCE_DIR}/include/glad/*.c"
    "${CMAKE_SOURCE_DIR}/include/stb/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/imgui/*.cpp"
    "${CMAKE_SOURCE_DIR}/include/imgui/backends/*.cpp"
)

# 添加可执行文件
add_executable(test ${SOURCES})

# 设置库的路径
if (APPLE)
    set(LIB_GLFW "${CMAKE_SOURCE_DIR}/include/GLFW/lib-MACOS/libglfw3.a")
    set(LIB_freetype "${CMAKE_SOURCE_DIR}/include/freetype/lib-MACOS/libfreetype.a")
    # set(LIB_assimp "${CMAKE_SOURCE_DIR}/include/assimp/lib-MACOS/libassimp.a")
elseif (WIN32)
    set(LIB_GLFW "${CMAKE_SOURCE_DIR}/include/GLFW/lib-WIN32/libglfw3.a")
    set(LIB_freetype "${CMAKE_SOURCE_DIR}/include/freetype/lib-WIN32/libfreetype.a")
endif()

# 链接库
if (APPLE)
    target_link_libraries(test ${LIB_GLFW} ${BZIP2_LIBRARIES} ${ZLIB_LIBRARIES} "-framework Cocoa -framework IOKit -framework CoreVideo -framework OpenGL")
elseif (WIN32)
    target_link_libraries(test ${LIB_GLFW} ${LIB_freetype} opengl32)
endif()