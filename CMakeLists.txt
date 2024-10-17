cmake_minimum_required(VERSION 3.20)
project(
    NeonEngine
    DESCRIPTION "Neon Engine"
    VERSION 0.1.0
    LANGUAGES C CXX
)

# ---------------------------------------------------------------------------------
# Dependencies

# assimp
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(lib/assimp)

# glfw
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)
# ---------------------------------------------------------------------------------

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
set(GLAD_LIBRARIES dl)

# ---------------------------------------------------------------------------------
# Build main application
file(GLOB MAIN_SOURCES
    NeonEngine/src/*.cpp

    lib/glad/src/glad.c

    lib/imgui/imgui*.cpp
    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp

    lib/stb/stb_image.cpp
)
add_executable(${PROJECT_NAME}
    ${MAIN_SOURCES}
)
include_directories(
    NeonEngine/src/

    lib/assimp/include/

    lib/glad/include/

    lib/glfw/include/

    lib/glm/

    lib/stb/

    lib/imgui/
    lib/imgui/backends/
)

#add_definitions(-DGLM_ENABLE_EXPERIMENTAL)

target_link_libraries(${PROJECT_NAME}
    assimp
    glfw
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/NeonEngine/fonts ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/fonts
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/NeonEngine/materials ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/materials
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/NeonEngine/models ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/models
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/NeonEngine/HDRIs ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/HDRIs
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/NeonEngine/shaders ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/shaders
    DEPENDS ${PROJECT_SHADERS})
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/NeonEngine/skyboxes ${CMAKE_BINARY_DIR}/${PROJECT_NAME}/skyboxes
)
