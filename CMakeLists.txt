cmake_minimum_required(VERSION 3.12.0)
project(GameEngine VERSION 0.1.0)

if(WIN32)
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/windows/${CMAKE_GENERATOR_PLATFORM})
set(CMAKE_CXX_STANDARD 20)
elseif(UNIX)
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/linux/${CMAKE_GENERATOR_PLATFORM})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20")
else()
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/other/${CMAKE_GENERATOR_PLATFORM})
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Debug)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/Release)
add_executable(GameEngine src/engine/main.cpp)

include(FetchContent)

#GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
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

#imgui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG v1.89.1
)

FetchContent_MakeAvailable(imgui)

if(NOT imgui_POPULATED)
    FetchContent_Populate(imgui)
endif()

file(GLOB IMGUI_SRCS ${FETCHCONTENT_BASE_DIR}/imgui-src/*.cpp)
list(APPEND IMGUI_SRCS ${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_opengl3.cpp ${FETCHCONTENT_BASE_DIR}/imgui-src/backends/imgui_impl_glfw.cpp)
add_library(imgui ${IMGUI_SRCS})
target_include_directories(imgui PUBLIC ${FETCHCONTENT_BASE_DIR}/imgui-src)
target_link_libraries(imgui glfw)

#Core Lib
file(GLOB CORE_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/*.cpp)
add_library(core ${CORE_SRCS})
target_include_directories(core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/core/includes)
target_link_libraries(core)

#Graphics Lib
file(GLOB GRAPHICS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/*.cpp)
file(GLOB GRAPHICS_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/*.ixx)
add_library(graphics ${GRAPHICS_SRCS} ${GRAPHICS_MODULES})
target_include_directories(graphics PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/graphics/includes)
target_link_libraries(graphics core libglew_static stb glm glfw)

#entity_component_system Lib
file(GLOB ECS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/ecs/*.cpp)
add_library(ecs ${ECS_SRCS})
target_include_directories(ecs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/ecs/includes)
target_link_libraries(ecs core imgui)

#Components Lib
file(GLOB COMPONENTS_MODULES ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/components/*.ixx)
add_library(components ${COMPONENTS_MODULES})
target_include_directories(components PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/components/includes)
target_link_libraries(components core glm ecs graphics glfw)

#Systems Lib
file(GLOB SYSTEMS_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/systems/*.cpp)
add_library(systems ${SYSTEMS_SRCS})
target_include_directories(systems PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/engine/systems/includes)
target_link_libraries(systems core glm components ecs imgui)

#Link libs to main executable
target_link_libraries(GameEngine glfw libglew_static glm ecs core systems imgui components graphics)

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
