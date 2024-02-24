#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

class Buffer {
public:
  Buffer(GLenum type, GLenum usage);
  ~Buffer();
  void bind();
  void bind(GLuint index);
  void unbind();
  void setData(void *data, size_t size);
  void setSubData(void *data, size_t size, size_t offset);
  void *map();
  void unmap();
  GLuint getID();
  GLuint getSize();

private:
  GLenum type;
  GLenum usage;
  GLuint id;
  GLuint size;
};
