#include <GL/glew.h>
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <string>

std::string readShader(std::string path) {
  std::string content;
  std::ifstream file;
  file.open(path);

  if(!file.is_open()) {
    std::cerr << "Could not read file " << path << std::endl;
    return "";
  }

  std::string line = "";
  while(!file.eof()) {
    std::getline(file, line);
    content.append(line + "\n");
  }
  file.close();

  return content;
}

GLuint loadShader(std::string path, GLenum shader_type) {

  GLuint shader = glCreateShader(shader_type);
  std::string content = readShader(path);
  const char* source = content.c_str();
  glShaderSource(shader, 1, &source, NULL);
  glCompileShader(shader);

  int success;
  char infoLog[512];
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if(!success)
  {
    glGetShaderInfoLog(shader, 512, NULL, infoLog);
    std::cout << "Failed to compile " << path << ": " << infoLog << std::endl;
    exit(1);
  };

  return shader;
}
GLuint createProgram(int n, ...) {
  GLuint program = glCreateProgram();
  
  va_list ptr;

  va_start(ptr, n);
  for (int i = 0; i < n; i++) {
    GLuint shader = va_arg(ptr, GLuint);

    glAttachShader(program, shader);
  }
  va_end(ptr);

  glLinkProgram(program);

  int success;
  char infoLog[512];
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(program, 512, NULL, infoLog);
    printf("Failed to link program: %s\n", infoLog);
    exit(1);
  }

  return program;
}

