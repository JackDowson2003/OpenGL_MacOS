cmake_minimum_required(VERSION 3.16)

project(OPENGL_CPP_MAC LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS 架构
if (APPLE)
    set(CMAKE_OSX_ARCHITECTURES arm64)
endif()

# =========================
# GLAD (静态库)
# =========================
add_library(glad STATIC
        dependencies/GLAD/src/glad.c
        )

target_include_directories(glad PUBLIC
        ${CMAKE_SOURCE_DIR}/dependencies/GLAD/include
        )

# =========================
# glfw (静态库)
# =========================
add_library(glfw STATIC IMPORTED)

set_target_properties(glfw PROPERTIES
        IMPORTED_LOCATION
        ${CMAKE_SOURCE_DIR}/dependencies/GLFW/lib-arm64/libglfw3.a
        INTERFACE_INCLUDE_DIRECTORIES
        ${CMAKE_SOURCE_DIR}/dependencies/GLFW/include
        )

# =========================
# 可执行文件
# =========================
add_executable(OPENGL_CPP_MAC
        src/Application.cpp
        src/Renderer.cpp
        src/VertexBuffer.cpp
        src/IndexBuffer.cpp
        src/VertexArray.cpp
        src/Shader.cpp src/Shader.h)

# =========================
# 链接（全部静态）
# =========================
target_link_libraries(OPENGL_CPP_MAC PRIVATE
        glad
        glfw
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        )

# =========================
# 编译选项
# =========================
target_compile_options(OPENGL_CPP_MAC PRIVATE
        -Wno-deprecated-declarations
        -Wno-nullability-completeness
        )

set(CMAKE_CXX_CLANG_TIDY "")
