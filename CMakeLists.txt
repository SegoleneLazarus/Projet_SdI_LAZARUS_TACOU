cmake_minimum_required(VERSION 3.11)
project(OpenGLTemplate)

add_executable(${PROJECT_NAME})

# Get source files
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "src/*")
# link files to project target
target_sources(${PROJECT_NAME} PRIVATE ${SRC_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "src/")

# Enable C++17
set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

# Enable good warnings
if (MSVC)
   target_compile_options(${PROJECT_NAME} PRIVATE /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors)
endif()

# Set the folder where the executable will be created
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

# Configure assets header file
# tips found here : https://shot511.github.io/2018-05-29-how-to-setup-opengl-project-with-cmake/
configure_file(src/helpers/RootDir.hpp.in src/helpers/RootDir.hpp)
include_directories(${CMAKE_BINARY_DIR}/src)

# Librairies
# ---Add GLFW---
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

# ---Add glad---
add_library(glad third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glad)