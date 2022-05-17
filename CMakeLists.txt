cmake_minimum_required(VERSION 3.0.0)
project(2DGameEngine VERSION 0.1.0)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
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

    set(ONLY_LIBS ON)
endif()

#glm
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

FetchContent_MakeAvailable(glm)

if(NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

#STB Image
file(MAKE_DIRECTORY ${FETCHCONTENT_BASE_DIR}/stb/includes/stb)
file(DOWNLOAD https://raw.githubusercontent.com/nothings/stb/master/stb_image.h ${FETCHCONTENT_BASE_DIR}/stb/includes/stb/stb_image.hpp)
add_library(stb INTERFACE)
target_include_directories(stb INTERFACE ${FETCHCONTENT_BASE_DIR}/stb/includes)

#Graphics Lib
file(GLOB GRAPHICS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/*.cpp)
add_library(graphics ${GRAPHICS_SRCS})
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/includes)
target_link_libraries(graphics libglew_static stb glm glfw)

#Input Lib
file(GLOB INPUT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/*.cpp)
add_library(input ${INPUT_SRCS})
target_include_directories(input PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/input/includes)
target_link_libraries(input glfw glm)

#Link libs to main executable
target_link_libraries(2DGameEngine glfw libglew_static graphics input glm)

#Add shaders to build dir
file(GLOB SHADER_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/shaders/*)
file(MAKE_DIRECTORY 
${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders
${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders
)
file(COPY ${SHADER_SRCS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/shaders)
file(COPY ${SHADER_SRCS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/shaders)

#Add assets to build dir
file(GLOB ASSET_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/assets/*)
file(MAKE_DIRECTORY 
${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets
${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/assets
)
file(COPY ${ASSET_SRCS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/assets)
file(COPY ${ASSET_SRCS} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/assets)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
