#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

class Texture {
public:
  Texture(int width, int height, GLenum slot);
  ~Texture();
  void bind();
  void unbind();
  void resize(int width, int height);
  GLuint getID();

private:
  GLuint id;
  GLenum slot;
  int width;
  int height;
};
