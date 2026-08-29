cmake_minimum_required(VERSION 3.16)
project(Lab3 LANGUAGES C CXX)

# ---- Toolchain / defaults ----
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_BUILD_TYPE Debug)             # change to Release if you want
set(CMAKE_OSX_ARCHITECTURES arm64)      # Apple Silicon (M1/M2/M3)


# Help CMake find Homebrew packages
# (first item is Apple Silicon default, second is Intel fallback)
list(PREPEND CMAKE_PREFIX_PATH /opt/homebrew /usr/local)

# Silence macOS OpenGL deprecation warnings in your sources (optional but nice)
add_compile_definitions(GL_SILENCE_DEPRECATION)

# ---- Dependencies (Homebrew) ----
find_package(OpenGL REQUIRED)           # provides OpenGL::GL (framework on macOS)
find_package(GLEW CONFIG REQUIRED)      # provides GLEW::GLEW
find_package(glfw3 CONFIG REQUIRED)     # provides glfw

# ---- Common helper libraries ----
add_library(readfile ../common/readfile.cpp)
add_library(lodepng ../common/lodepng.cpp)

# Expose ../common headers to anything linking readfile/lodepng
target_include_directories(readfile PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common)
target_include_directories(lodepng PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../common)

# ---- Executables ----
add_executable(lab3-1 lab3-1.cpp)
add_executable(lab3-2 lab3-2.cpp)
add_executable(lab3-3 lab3-3.cpp)
add_executable(lab3-4 lab3-4.cpp)


find_path(GLM_INCLUDE_DIR glm/vec3.hpp
    PATHS /opt/homebrew/include /usr/local/include
    REQUIRED
)

# ---- Link everything the modern way ----
set(COMMON_LIBS
    OpenGL::GL
    GLEW::GLEW
    glfw
    lodepng
    readfile
)

foreach(tgt lab3-1 lab3-2 lab3-3 lab3-4)
    target_link_libraries(${tgt} PRIVATE ${COMMON_LIBS})
endforeach()
