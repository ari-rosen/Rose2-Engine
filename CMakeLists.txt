cmake_minimum_required(VERSION 3.20)
project(RoseV2 VERSION 1.0 LANGUAGES CXX)

# --- Global Settings --- #
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Configurable Options --- #
option(ROSEV2_BUILD_SHARED "Build RoseV2 as a shared library" ON)
option(ROSEV2_USE_GLFW "Use GLFW as the window backend" ON)
option(ROSEV2_BUILD_TESTS "Build RoseV2 unit tests" ON)
option(ROSEV2_USE_OPENGL "Use OpenGL as the Graphics API" ON)

# --- Dependencies --- #
if(ROSEV2_USE_GLFW)
    set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
    set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
    set(GLFW_SHARED OFF CACHE BOOL "" FORCE)

    add_subdirectory(external/glfw-3.4)
    message(STATUS "RoseV2: Using GLFW backend")
endif()

if(ROSEV2_USE_OPENGL)
    add_subdirectory(external/glad)
    message(STATUS "RoseV2: Using OpenGL Graphics API")
endif()

# --- Collect Sources --- #
set(ROSEV2_SOURCES
    src/RoseV2/Core/Application.cpp
    src/RoseV2/Core/Window.cpp
    src/RoseV2/Core/Logger.cpp
    
    src/RoseV2/ECS/LifetimeManager.cpp
    
    src/RoseV2/Graphics/GraphicsContext.cpp
    src/RoseV2/Graphics/Renderer.cpp
    src/RoseV2/Graphics/RendererAPI.cpp
    src/RoseV2/Graphics/RenderCommand.cpp
    src/RoseV2/Graphics/Shader.cpp
)

if(ROSEV2_USE_GLFW)
    list(APPEND ROSEV2_SOURCES
        src/RoseV2/Platform/GLFWWindow.cpp
    )
endif()

if(ROSEV2_USE_OPENGL)
    list(APPEND ROSEV2_SOURCES
        src/RoseV2/Platform/OpenGL/OpenGLGraphicsContext.cpp
        src/RoseV2/Platform/OpenGL/OpenGLRendererAPI.cpp
        src/RoseV2/Platform/OpenGL/OpenGLShader.cpp
    )
endif()

# --- Library Target --- #
if(ROSEV2_BUILD_SHARED)
    add_library(RoseV2 SHARED ${ROSEV2_SOURCES})
else()
    add_library(RoseV2 STATIC ${ROSEV2_SOURCES})
endif()

target_include_directories(RoseV2 PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include> 
)

if(ROSEV2_USE_GLFW)
    target_compile_definitions(RoseV2 PUBLIC ROSEV2_USE_GLFW)
    target_link_libraries(RoseV2 PUBLIC glfw)
endif()

if(ROSEV2_USE_OPENGL)
    target_compile_definitions(RoseV2 PUBLIC ROSEV2_USE_OPENGL)
    target_link_libraries(RoseV2 PUBLIC glad)
endif()

# --- Misc --- #
target_include_directories(RoseV2 PUBLIC
    ${PROJECT_SOURCE_DIR}/external/glm
)

target_compile_definitions(RoseV2 PUBLIC PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}")

# --- Platform-specific Linking --- #
if(APPLE)
    target_link_libraries(RoseV2 PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework Foundation"
    )
elseif(UNIX)
    find_package(OpenGL REQUIRED)
    target_link_libraries(RoseV2 PRIVATE OpenGL::GL)
endif()

# --- PCH --- #
target_precompile_headers(RoseV2 PRIVATE
    "${CMAKE_CURRENT_SOURCE_DIR}/include/RoseV2/pch.hpp"
)

# --- Tests --- #

if(ROSEV2_BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    add_executable(RoseV2Tests
        tests/ComponentManager.test.cpp
        tests/LifetimeManager.test.cpp
    )

    target_link_libraries(RoseV2Tests
        PRIVATE
            RoseV2
            gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(RoseV2Tests)
endif()
