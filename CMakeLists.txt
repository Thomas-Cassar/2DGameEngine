cmake_minimum_required(VERSION 3.0.0)
project(2DGameEngine VERSION 0.1.0)

add_executable(2DGameEngine src/engine/main.cpp)

include(FetchContent)

#GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.6
)

FetchContent_MakeAvailable(glfw)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)

    set(GLFW_BUILD_EXAMPLES OFF)
    set(GLFW_BUILD_TESTS OFF)
    set(GLFW_BUILD_DOCS OFF)
    set(GLFW_INSTALL OFF)

    add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})
endif()

#GLEW
FetchContent_Declare(
    glew-cmake
    GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
    GIT_TAG glew-cmake-2.2.0
)

FetchContent_MakeAvailable(glew-cmake)

if(NOT glew-cmake_POPULATED)
    FetchContent_Populate(glew-cmake)
endif()

#Graphics Lib
file(GLOB GRAPHICS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/*.cpp)
add_library(graphics ${GRAPHICS_SRCS})
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/includes)
target_link_libraries(graphics libglew_static)

#Link libs to main executable
target_link_libraries(2DGameEngine glfw libglew_static graphics)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
