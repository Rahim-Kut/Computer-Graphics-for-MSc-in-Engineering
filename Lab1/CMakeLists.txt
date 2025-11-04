cmake_minimum_required (VERSION 3.3)

project(Lab1)

set(CMAKE_BUILD_TYPE Debug)

#set(CMAKE_BUILD_TYPE Release)

set(OpenGL_GL_PREFERENCE GLVND)


if (WIN32)

 list(APPEND CMAKE_PREFIX_PATH "../common/windows_users/GLEW/glew-2.0.0")

 set(GLEW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../common/windows_users/GLEW/glew-2.0.0/include/)

 set(GLEW_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../common/windows_users/GLEW/glew-2.0.0/lib/Release/x64/glew32s.lib)

 set(GLFW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../common/windows_users/GLFW/64bit/glfw-3.2.1.bin.WIN64/include/)

 #set(GLFW_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../common/windows_users/GLFW/64bit/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.lib)

 set(GLFW_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/../common/windows_users/GLFW/64bit/glfw-3.2.1.bin.WIN64/lib-vc2015/glfw3.lib)

 set(GLM_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../common/windows_users/glm/)

else(WIN32)

 find_package(PkgConfig REQUIRED)

 pkg_search_module(GLFW REQUIRED glfw3)

endif (WIN32)


find_package(OpenGL REQUIRED)

find_package(GLEW REQUIRED)


add_library(readfile ../common/readfile.cpp)

add_library(lodepng ../common/lodepng.cpp)


include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../common/)

include_directories(${OPENGL_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS} ${GLM_INCLUDE_DIRS})

set(common_libs ${OPENGL_LIBRARIES} ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} lodepng readfile)


add_executable(lab1-1 lab1-1.cpp)

add_executable(lab1-2 lab1-2.cpp)

add_executable(lab1-3 lab1-3.cpp)

add_executable(lab1-4 lab1-4.cpp)

add_executable(lab1-5 lab1-5.cpp)

add_executable(lab1-6 lab1-6.cpp)

add_executable(lab1-7 lab1-7.cpp)

add_executable(lab1-8 lab1-8.cpp)

add_executable(lab1-9 lab1-9.cpp)


target_link_libraries(lab1-1 ${common_libs})

target_link_libraries(lab1-2 ${common_libs})

target_link_libraries(lab1-3 ${common_libs})

target_link_libraries(lab1-4 ${common_libs})

target_link_libraries(lab1-5 ${common_libs})

target_link_libraries(lab1-6 ${common_libs})

target_link_libraries(lab1-7 ${common_libs})

target_link_libraries(lab1-8 ${common_libs})

target_link_libraries(lab1-9 ${common_libs})