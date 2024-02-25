#pragma once

// clang-format off
#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
// clang-format on

#include "shader.hpp"
#include <string>
#include <vector>

class ShaderLoader {
public:
  ShaderLoader();
  void addIncludePath(std::string path);
  std::string read(std::string path);
  Shader load(std::string path, GLenum type);

private:
  std::vector<std::string> include_paths;

  std::string getInclude(std::string include);
};
