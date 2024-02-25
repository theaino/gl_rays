#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include <string>

class Shader {
public:
  Shader(GLenum type);
  ~Shader();
  void source(std::string content);
  int compile();
  void logErrors();
  GLuint getID();

private:
  GLuint id;
  GLenum type;
};

std::string readShader(std::string path);

GLuint createShader(std::string content, GLenum shader_type);
