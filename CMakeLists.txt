cmake_minimum_required(VERSION 3.20)
project(
    NeonEngine
    DESCRIPTION "Neon Engine"
    VERSION 0.1.0
    LANGUAGES C CXX
)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
option(BUILD_SHARED_LIBS OFF)
add_subdirectory(lib/assimp)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)

set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/${PROJECT_NAME}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
set(GLAD_LIBRARIES dl)

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

file(GLOB VENDORS_SOURCES 
    lib/glad/src/glad.c

    lib/imgui/backends/imgui_impl_glfw.cpp
    lib/imgui/backends/imgui_impl_opengl3.cpp
    lib/imgui/imgui*.cpp
)

file(GLOB PROJECT_SOURCES
    NeonEngine/src/*.cpp
)

file(GLOB PROJECT_SHADERS
    NeonEngine/shaders/*.vert
    NeonEngine/shaders/*.frag
)

source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("sources" FILES ${PROJECT_SOURCES})
source_group("vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE)

add_executable(
    ${PROJECT_NAME}
    ${PROJECT_SOURCES}
    ${PROJECT_SHADERS} 
    ${VENDORS_SOURCES}
)

target_link_libraries(${PROJECT_NAME}
    assimp
    glfw
    ${GLFW_LIBRARIES}
    ${GLAD_LIBRARIES}
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
