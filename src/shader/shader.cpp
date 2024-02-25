// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on
#include <iostream>
#include <string>

#include "shader.hpp"

Shader::Shader(GLenum type) {
  this->type = type;
  this->id = glCreateShader(type);
}

Shader::~Shader() { glDeleteShader(this->id); }

void Shader::source(std::string content) {
  const char *source = content.c_str();
  glShaderSource(this->id, 1, &source, NULL);
}

int Shader::compile() {
  glCompileShader(this->id);
  int success;
  glGetShaderiv(this->id, GL_COMPILE_STATUS, &success);
  return success;
}

void Shader::logErrors() {
  char infoLog[512];
  glGetShaderInfoLog(this->id, 512, NULL, infoLog);
  std::cout << "Failed to compile " << this->type << ": " << infoLog
            << std::endl;
}

GLuint Shader::getID() { return this->id; }
