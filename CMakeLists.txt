cmake_minimum_required(VERSION 3.5)
project(gl_rays)

# Setup

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Files

file(GLOB_RECURSE sources "src/*.cpp" "src/*.hpp")

# Target

include_directories(${PROJECT_SOURCE_DIR}/src)
add_executable(gl_rays ${sources})
#target_compile_options(gl_rays PUBLIC -Wall -Wfloat-conversion)

# Dependencies

find_package(Freetype REQUIRED)
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(imgui CONFIG REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIR}
  )

target_link_libraries(gl_rays PRIVATE
  Freetype::Freetype
  GLEW::GLEW
  glfw
  glm::glm
  imgui::imgui
)

