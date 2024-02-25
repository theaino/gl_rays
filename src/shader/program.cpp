// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "program.hpp"
#include "shader.hpp"
#include <iostream>
#include <vector>

Program::Program(std::vector<Shader> shaders) {
  this->id = glCreateProgram();
  for (Shader shader : shaders) {
    glAttachShader(id, shader.getID());
  }
  int success;
}

int Program::link() {
  glLinkProgram(id);
  int success;
  glGetProgramiv(id, GL_LINK_STATUS, &success);
  return success;
}

void Program::use() { glUseProgram(id); }

void Program::logErrors() {
  char info_log[512];
  glGetProgramInfoLog(id, 512, NULL, info_log);
  std::cout << "Failed to link program: " << info_log << std::endl;
}

GLuint Program::getID() { return id; }

Program::~Program() { glDeleteProgram(id); }
