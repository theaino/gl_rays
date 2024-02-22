cmake_minimum_required(VERSION 3.20)

project(gl_rays)

find_package(PkgConfig REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

file(GLOB_RECURSE gl_rays_SRC CONFIGURE_DEPENDS "src/*.cpp")

file(GLOB IMGUI_SRC CONFIGURE_DEPENDS
  "external/imgui/*.cpp"
  "external/imgui/backends/imgui_impl_glfw.cpp"
  "external/imgui/backends/imgui_impl_opengl3.cpp"
  )

add_executable(gl_rays ${gl_rays_SRC} ${IMGUI_SRC})

pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLEW REQUIRED glew)

find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(Freetype REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${FREETYPE_INCLUDE_DIRS})
include_directories(${IMGUI_INCLUDE_DIRS})

include_directories(${PROJECT_SOURCE_DIR}/external/imgui)
include_directories(${PROJECT_SOURCE_DIR})

target_link_libraries(gl_rays PRIVATE freetype)
target_link_libraries(gl_rays PRIVATE ${OPENGL_LIBRARIES})
target_link_libraries(gl_rays PRIVATE ${GLEW_LIBRARIES})
target_link_libraries(gl_rays PRIVATE ${GLFW_LIBRARIES})
target_link_libraries(gl_rays PRIVATE glm::glm)

target_include_directories(gl_rays PRIVATE imgui)

target_include_directories(gl_rays PRIVATE
  ${CMAKE_SOURCE_DIR}/src
 )

