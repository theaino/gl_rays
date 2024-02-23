#include "loader.hpp"
#include <fstream>
#include <iostream>

ShaderLoader::ShaderLoader() {}

void ShaderLoader::addIncludePath(std::string path) {
  include_paths.push_back(path);
}

std::string ShaderLoader::read(std::string path) {
  std::string content;
  std::ifstream file;
  file.open(getInclude(path));

  if (!file.is_open()) {
    std::cerr << "Could not read file " << path << std::endl;
    return "";
  }

  std::string line = "";
  while (!file.eof()) {
    std::getline(file, line);
    if (line.rfind("#include", 0) == 0) {
      std::string include_path = line.substr(9);

      std::string include_source = read(include_path);
      content.append(read(include_path));
      continue;
    }
    content.append(line + "\n");
  }
  file.close();

  return content;
}

std::string ShaderLoader::getInclude(std::string include) {
  for (std::string path : include_paths) {
    std::string file_path = path + include;
    std::ifstream file;
    file.open(path + include);
    if (file.is_open()) {
      file.close();
      return file_path;
    }
  }
  return "";
}
