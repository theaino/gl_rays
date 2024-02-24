#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "buffer.hpp"

class UBO : public Buffer {
public:
  UBO(GLenum usage);
  void bindUniformBlock(GLuint program, const char *name, GLuint binding);
};
