#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <vector>
// clang-format on
#include "shader.hpp"

class Program {
public:
  Program(std::vector<Shader> shaders);
  ~Program();
  int link();
  void use();
  void logErrors();
  GLuint getID();

private:
  GLuint id;
};
