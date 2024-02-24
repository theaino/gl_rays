// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "ubo.hpp"

UBO::UBO(GLenum usage) : Buffer(GL_UNIFORM_BUFFER, usage) {}

void UBO::bindUniformBlock(GLuint program, const char *name, GLuint binding) {
  GLuint index = glGetUniformBlockIndex(program, name);
  glUniformBlockBinding(program, index, binding);
  glBindBufferBase(GL_UNIFORM_BUFFER, binding, getID());
}
