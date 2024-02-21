#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include <string>

std::string readShader(std::string path);

GLuint loadShader(std::string path, GLenum shader_type);

GLuint createProgram(int n, ...);
