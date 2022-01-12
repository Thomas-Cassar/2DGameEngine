cmake_minimum_required(VERSION 3.0.0)
project(2DGameEngine VERSION 0.1.0)

include(CTest)
enable_testing()

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
    add_subdirectory(${glew-cmake_SOURCE_DIR} ${glew-cmake_BINARY_DIR} ${glew-cmake_INCLUDE_DIR})
    target_include_directories(2DGameEngine ${glew-cmake_INCLUDE_DIR})
endif()

target_link_libraries(2DGameEngine glfw libglew_static)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
