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

# --- Collect Sources --- #
set(ROSEV2_SOURCES
    src/RoseV2/Core/Application.cpp
    src/RoseV2/Core/Application2D.cpp
    src/RoseV2/Core/Window.cpp
    src/RoseV2/Core/Logger.cpp

    src/RoseV2/ECS/ComponentArray.cpp
)

if(ROSEV2_USE_GLFW)
    list(APPEND ROSEV2_SOURCES
        src/RoseV2/Platform/GLFWWindow.cpp
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
