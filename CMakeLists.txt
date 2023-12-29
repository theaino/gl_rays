cmake_minimum_required(VERSION 3.20)

project(gl_rays)

find_package(PkgConfig REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

add_executable(gl_rays ${PROJECT_SOURCE_DIR}/src/main.cpp)

pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(gl_rays ${OPENGL_LIBRARIES})
target_link_libraries(gl_rays ${GLEW_LIBRARIES})
target_link_libraries(gl_rays ${GLFW_LIBRARIES})

target_include_directories(gl_rays PRIVATE
  ${CMAKE_SOURCE_DIR}/src
 )

