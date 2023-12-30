#include <GL/glew.h>
#include <GL/gl.h>

GLuint createTexture(int width, int height, GLenum slot) {
  GLuint texture;

  glGenTextures(1, &texture);
  glActiveTexture(slot);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);

  glBindImageTexture(slot - GL_TEXTURE0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

  return texture;
}

