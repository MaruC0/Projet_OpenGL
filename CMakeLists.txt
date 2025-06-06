cmake_minimum_required(VERSION 3.10)

project(OpenGLProgram)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set directories
set(SRC_DIR src)
set(INCLUDE_DIR include)
set(EXTERNAL_DIR external)
set(GLEW_DIR ${EXTERNAL_DIR}/glew-cmake)
set(GLFW_DIR ${EXTERNAL_DIR}/glfw)
set(GLM_DIR ${EXTERNAL_DIR}/glm)
set(ASSIMP_DIR ${EXTERNAL_DIR}/assimp)

# Include directories
include_directories(${INCLUDE_DIR})
include_directories(${GLEW_DIR}/include)
include_directories(${GLFW_DIR}/include)
include_directories(${GLM_DIR})
include_directories(${ASSIMP_DIR}/include)

# Source files
set(SOURCES
    ${SRC_DIR}/main.cpp
    ${SRC_DIR}/node.cpp
    ${SRC_DIR}/shape.cpp
    ${SRC_DIR}/shader.cpp
    ${SRC_DIR}/viewer.cpp
    ${SRC_DIR}/triangle.cpp
    ${SRC_DIR}/cylinder.cpp
    ${SRC_DIR}/skybox.cpp
    ${SRC_DIR}/texture.cpp
    ${SRC_DIR}/textured_triangle.cpp
    ${SRC_DIR}/skybox.cpp
    ${SRC_DIR}/ground.cpp
    ${SRC_DIR}/utils.cpp
)

# Define Shader directory
add_compile_definitions(SHADER_DIR="${CMAKE_CURRENT_SOURCE_DIR}/shaders/")

# Define Texture directory
add_compile_definitions(TEX_DIR="${CMAKE_CURRENT_SOURCE_DIR}/textures/")

# Define Models directory
add_compile_definitions(MODEL_DIR="${CMAKE_CURRENT_SOURCE_DIR}/models/")

# GLEW library
add_subdirectory(${GLEW_DIR})
set(LIBS ${LIBS} libglew_static)

# GLFW library
add_subdirectory(${GLFW_DIR})
set(LIBS ${LIBS} glfw)

# GLM library
add_subdirectory(${GLM_DIR})

# Assimp library
add_subdirectory(${ASSIMP_DIR})
set(LIBS ${LIBS} assimp)

# Executable
add_executable(opengl_program ${SOURCES})
target_link_libraries(opengl_program ${LIBS})