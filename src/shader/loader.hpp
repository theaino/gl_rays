#pragma once

#include <string>
#include <vector>

class ShaderLoader {
public:
  ShaderLoader();
  void addIncludePath(std::string path);
  std::string read(std::string path);

private:
  std::vector<std::string> include_paths;

  std::string getInclude(std::string include);
};
