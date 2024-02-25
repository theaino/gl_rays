// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "texture.hpp"

Texture::Texture(int width, int height, GLenum slot) {
  glGenTextures(1, &id);
  bind();
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
               GL_FLOAT, 0);

  glBindImageTexture(slot - GL_TEXTURE0, id, 0, GL_FALSE, 0, GL_READ_WRITE,
                     GL_RGBA32F);
  this->slot = slot;
  this->width = width;
  this->height = height;
}

Texture::~Texture() { glDeleteTextures(1, &id); }

void Texture::bind() {
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::unbind() { glBindTexture(GL_TEXTURE_2D, 0); }

void Texture::resize(int width, int height) {
  this->width = width;
  this->height = height;
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA,
               GL_FLOAT, 0);
}

GLuint Texture::getID() { return id; }
